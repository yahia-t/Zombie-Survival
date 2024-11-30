#include "model_loader.h"
#define STB_IMAGE_IMPLEMENTATION // Enable stb_image implementation
#include "stb_image.h"

#include <glew.h>

#include <glut.h>
#include <iostream>
#include <map> // For caching textures

std::map<std::string, unsigned int> textureCache; // To avoid loading the same texture multiple times

unsigned int loadTextureFromFile(const std::string& filename, const std::string& directory) {
    std::string filepath = directory + "/" + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cerr << "Texture failed to load at path: " << filepath << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


std::vector<Mesh> meshes;

void processNode(aiNode* node, const aiScene* scene);
void processMesh(aiMesh* mesh, const aiScene* scene);

void loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        vertices.push_back(mesh->mNormals[i].x);
        vertices.push_back(mesh->mNormals[i].y);
        vertices.push_back(mesh->mNormals[i].z);

        if (mesh->mTextureCoords[0]) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    unsigned int textureID = 0;
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString str;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS) {
            std::string texturePath = str.C_Str();

            if (scene->HasTextures()) {
                auto embeddedTexture = scene->GetEmbeddedTexture(texturePath.c_str());
                if (embeddedTexture) {
                    glGenTextures(1, &textureID);
                    glBindTexture(GL_TEXTURE_2D, textureID);

                    if (embeddedTexture->mHeight == 0) {
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, embeddedTexture->mWidth, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, embeddedTexture->pcData);
                    }
                    else {
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, embeddedTexture->mWidth, embeddedTexture->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, embeddedTexture->pcData);
                    }

                    glGenerateMipmap(GL_TEXTURE_2D);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                }
            }
            else {
                if (textureCache.find(texturePath) == textureCache.end()) {
                    textureID = loadTextureFromFile(texturePath, "assets/");
                    textureCache[texturePath] = textureID;
                }
                else {
                    textureID = textureCache[texturePath];
                }
            }
            // Debug texture loading
            std::cout << "Texture loaded: " << textureID << " for path: " << texturePath << std::endl;
        }
    }

    meshes.push_back({ VAO, VBO, EBO, (unsigned int)indices.size(), textureID });

    // Debug mesh information
    std::cout << "Mesh added with " << indices.size() << " indices, VAO: " << VAO << ", TextureID: " << textureID << std::endl;
}


void renderModel() {
    // Push the current matrix stack to apply transformations only to the model
    glPushMatrix();

    // Apply scaling
    glScalef(0.1f, 0.1f, 0.1f); // Adjust scale to fit your model in the scene

    // Apply rotation to make the model upright
    glRotatef(90.0f, -1.0f, 0.0f, 0.0f); // Rotate around the X-axis
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // Rotate around the Z-axis

    // Apply translation if needed (position the model)
    glTranslatef(0.0f, 0.0f, 3.0f); // Adjust position based on your scene setup

    // Iterate through all meshes and render them
    for (const auto& mesh : meshes) {
        if (mesh.textureID != 0) {
            // Enable 2D texture mapping
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mesh.textureID);
        }

        // Bind the vertex array object
        glBindVertexArray(mesh.VAO);

        // Render the mesh
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);

        // Unbind the vertex array object
        glBindVertexArray(0);

        // Disable textures for the next render pass if necessary
        glDisable(GL_TEXTURE_2D);
    }

    // Restore the previous matrix stack
    glPopMatrix();
}



