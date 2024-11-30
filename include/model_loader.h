#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <vector>
#include <string>
#include <glew.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

struct Mesh {
    unsigned int VAO;       // Vertex Array Object
    unsigned int VBO;       // Vertex Buffer Object
    unsigned int EBO;       // Element Buffer Object
    unsigned int indexCount; // Number of indices
    unsigned int textureID;  // Texture ID for the mesh
};

void loadModel(const std::string& path);
void renderModel();

#endif
