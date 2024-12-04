#include <iostream>
#include <glew.h>
#include <glut.h>
#include "camera.h"
#include "player.h"
#include "model_loader.h"
#include "main.h"
#include "zombie.h"
#include <vector>
#include "projectile.h"
#include "powerup.h"
#include <vector>
#include <chrono> // For timing





ProjectileType currentProjectileType = ProjectileType::BASIC;

#define PI 3.14159265359

#define DEG2RAD(a) (a * 0.0174532925)

std::vector<Projectile> projectiles; // Container for active projectiles

std::vector<PowerUp> powerUps;
std::vector<Zombie> zombies;
float zombieSpawnInterval = 5.0f;  // Time between spawns
float zombieSpawnTimer = 0.0f;


Player player;
Cam camera;


float rotAng = 0.0f;
bool keys[256] = { false };  // Key state tracking
bool isMouseLocked = true;   // Flag to lock the mouse inside
bool isThirdPerson = true;   // Third-person/first-person toggle
int windowWidth = 1200;
int windowHeight = 800;
float moveSpeed = 0.3f;
int score = 0;
float powerUpTimer = 0.0f;
const float powerUpDuration = 10.0f; // Power-up lasts 10 seconds


void renderExplosions() {
    for (size_t i = 0; i < Projectile::explosions.size(); ) {
        const auto& [explosionCenter, duration] = Projectile::explosions[i];

        // Draw explosion as a semi-transparent sphere
        glPushMatrix();
        glTranslatef(explosionCenter.x, explosionCenter.y, explosionCenter.z);
        glColor4f(1.0f, 0.5f, 0.0f, 0.5f); // Orange, semi-transparent
        glutWireSphere(5.0f, 16, 16);      // Radius 5.0, segments 16x16
        glPopMatrix();

        // Reduce duration
        Projectile::explosions[i].second -= 0.01f; // Reduce lifetime per frame

        // Remove if duration is over
        if (Projectile::explosions[i].second <= 0.0f) {
            Projectile::explosions.erase(Projectile::explosions.begin() + i);
        }
        else {
            ++i;
        }
    }
}



void spawnPowerUps() {
    powerUps.emplace_back(Vector3f(rand() % 20 - 10, 0, rand() % 20 - 10), ProjectileType::BASIC);
    powerUps.emplace_back(Vector3f(rand() % 20 - 10, 0, rand() % 20 - 10), ProjectileType::SPREAD);
    powerUps.emplace_back(Vector3f(rand() % 20 - 10, 0, rand() % 20 - 10), ProjectileType::RAPID);
    powerUps.emplace_back(Vector3f(rand() % 20 - 10, 0, rand() % 20 - 10), ProjectileType::EXPLOSIVE);
    powerUps.emplace_back(Vector3f(rand() % 20 - 10, 0, rand() % 20 - 10), ProjectileType::HOMING);
}


void renderScore() {
    glColor3f(1.0f, 1.0f, 1.0f); // White text
    glRasterPos2f(-0.95f, 0.9f); // Position on screen
    std::string scoreText = "Score: " + std::to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}


// Implementations of functions declared in main.h

void spawnZombie() {
    Vector3f spawnPosition(rand() % 20 - 10, 0.0f, rand() % 20 - 10); // Random spawn position
    zombies.emplace_back(spawnPosition);
}

void fireProjectile() {
    Vector3f projectileDirection = Vector3f(
        sin(DEG2RAD(player.rotationAngle) + (PI / 2)),
        0,
        -cos(DEG2RAD(player.rotationAngle ) + (PI / 2))
    );

    switch (currentProjectileType) {
    case ProjectileType::BASIC:
        projectiles.emplace_back(player.position, projectileDirection, 0.2f, 20.0f, ProjectileType::BASIC);
        break;

    case ProjectileType::SPREAD:
        // Fire three projectiles in a spread pattern
        projectiles.emplace_back(player.position, projectileDirection, 0.2f, 15.0f, ProjectileType::SPREAD);
        projectiles.emplace_back(player.position, projectileDirection.rotateY(-15.0f), 0.2f, 15.0f, ProjectileType::SPREAD);
        projectiles.emplace_back(player.position, projectileDirection.rotateY(15.0f), 0.2f, 15.0f, ProjectileType::SPREAD);
        break;

    case ProjectileType::RAPID:
        projectiles.emplace_back(player.position, projectileDirection, 0.4f, 10.0f, ProjectileType::RAPID);
        break;

    case ProjectileType::EXPLOSIVE:
        projectiles.emplace_back(player.position, projectileDirection, 0.1f, 50.0f, ProjectileType::EXPLOSIVE);
        break;

    case ProjectileType::HOMING:
        projectiles.emplace_back(player.position, projectileDirection, 0.15f, 20.0f, ProjectileType::HOMING);
        break;

    default:
        break;
    }
}



void Anim() {
    rotAng += 0.01f; // Increment rotation angle

    // Update zombies
    for (auto& zombie : zombies) {
        zombie.moveTowards(player.position);
    }

    for (size_t i = 0; i < powerUps.size(); ) {
        if (powerUps[i].isCollected(player.position)) {
            currentProjectileType = powerUps[i].type; // Update the projectile type
            powerUps.erase(powerUps.begin() + i);    // Remove the collected power-up
        }
        else {
            ++i;
        }
    }

    if (currentProjectileType != ProjectileType::BASIC) {
        powerUpTimer += 0.01f;
        if (powerUpTimer >= powerUpDuration) {
            currentProjectileType = ProjectileType::BASIC;
            powerUpTimer = 0.0f;
        }
    }



    // Spawn new zombies
    zombieSpawnTimer += 0.01f;
    if (zombieSpawnTimer >= zombieSpawnInterval) {
        spawnZombie();
        zombieSpawnTimer = 0.0f;
    }

    


    // Update projectiles
    for (size_t i = 0; i < projectiles.size(); ) {
        for (auto& projectile : projectiles) {
            projectile.update(zombies); // Pass the zombies list for homing logic
        }

        // Remove out-of-bounds projectiles
        if (projectiles[i].isOutOfBounds()) {
            projectiles.erase(projectiles.begin() + i);
        }
        else {
            ++i;
        }
    }

    // Check for collisions between projectiles and zombies
    for (size_t i = 0; i < projectiles.size(); ) {
        bool projectileHit = false;

        for (size_t j = 0; j < zombies.size(); ) {
            if (projectiles[i].position.distance(zombies[j].position) < 1.0f) { // Collision threshold
                zombies[j].takeDamage(projectiles[i].damage);

                if (zombies[j].isDead()) {
                    zombies.erase(zombies.begin() + j);
                    // Increase score (assume you have a score variable)
                    score++;
                }
                else {
                    ++j;
                }

                projectileHit = true;
                break;
            }
            else {
                ++j;
            }
        }

        if (projectileHit) {
            projectiles.erase(projectiles.begin() + i);
        }
        else {
            ++i;
        }
    }


    glutPostRedisplay(); // Request display update
}


void Reshape(int width, int height) {
    if (height == 0) height = 1; // Prevent division by zero
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height); // Update the OpenGL viewport
    cam(); // Update the camera's perspective
}

void MouseMovement(int x, int y) {
    if (isMouseLocked) {
        int dx = x - windowWidth / 2; // Calculate movement delta
        float sensitivity = 0.2f;     // Adjust sensitivity for smoother rotation
        player.rotate(dx * sensitivity); // Rotate player based on mouse movement

        centerMouse(); // Re-center the mouse
    }
    glutPostRedisplay(); // Request display update
}

void Keyboard(unsigned char key, int x, int y) {
    keys[key] = true; // Mark key as pressed

    if (key == 'v') { // Toggle between first- and third-person views
        isThirdPerson = !isThirdPerson;
    }
    if (key == 27) { // ASCII code for the Escape key
        exit(0); // Exit the application
    }
    if (key == 32) { // Space bar
        fireProjectile();
    }

}

void KeyboardUp(unsigned char key, int x, int y) {
    keys[key] = false; // Mark key as released
}




void Display() {
    // Clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    if (keys['w'] || keys['W']) { // Move forward
        player.moveForward(moveSpeed);
        std::cout << "Player Position: (" << player.position.x << ", "
            << player.position.y << ", " << player.position.z << ")\n";

    }
    if (keys['s'] || keys['S']) { // Move backward
        player.moveForward(-moveSpeed);
        std::cout << "Player Position: (" << player.position.x << ", "
            << player.position.y << ", " << player.position.z << ")\n";

    }
    if (keys['a'] || keys['A']) { // Rotate left
        player.moveRight(moveSpeed);
        std::cout << "Player Position: (" << player.position.x << ", "
            << player.position.y << ", " << player.position.z << ")\n";

    }
    if (keys['d'] || keys['D']) { // Rotate right
        player.moveRight(-moveSpeed);
        std::cout << "Player Position: (" << player.position.x << ", "
            << player.position.y << ", " << player.position.z << ")\n";

    }

    renderScore();
    renderExplosions();




    // Render power-ups
    for (const auto& powerUp : powerUps) {
        powerUp.render();
    }

    // Render projectiles
    for (const auto& projectile : projectiles) {
        projectile.render();
    }


    // Render zombies
    for (const auto& zombie : zombies) {
        zombie.render();
    }

    // Update camera position and orientation
    camera.followPlayer(isThirdPerson);
    cam();

    // Draw the ground plane
    glPushMatrix();
    glColor3f(0.8f, 0.8f, 0.8f); // Light gray color for the plane
    drawPlane(50.0f, 50);        // Plane size 50x50, divided into 50 segments
    glPopMatrix();

    // Render the loaded 3D model
    glPushMatrix();
    glTranslatef(player.position.x, player.position.y, player.position.z);
    glRotatef(player.rotationAngle, 0, -1, 0); // Rotate based on player's rotation
    renderModel(); // Call the renderModel function from model_loader.cpp
    glPopMatrix();

    // Swap buffers (for double buffering)
    glutSwapBuffers();
}



int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Use double buffering
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("Idol Zombie Survival");

    // Initialize GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "GLEW initialization error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    std::cout << "GLEW initialized successfully! Using OpenGL version: "
        << glGetString(GL_VERSION) << std::endl;

    // Setup display callbacks
    glutDisplayFunc(Display);
    glutIdleFunc(Anim); // Called when the system is idle
    glutReshapeFunc(Reshape);
    glutPassiveMotionFunc(MouseMovement);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);

    // Set up OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D); // Enable 2D textures

    // Load the 3D model
    loadModel("assets/penguin_skipper.glb");
    spawnPowerUps(); // Call this function to add power-ups to the game world



    // Center mouse
    centerMouse();
    glutSetCursor(GLUT_CURSOR_NONE); // Hide the cursor

    // Enter the GLUT event-processing loop
    glutMainLoop();

    return 0;
}