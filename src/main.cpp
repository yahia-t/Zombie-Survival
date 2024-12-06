#include <iostream>
#include <vector>
#include <chrono> // For Timing

#include <glew.h>
#include <glut.h>

#include "boss.h"
#include "camera.h"
#include "main.h"
#include "model_loader.h"
#include "player.h"
#include "powerup.h"
#include "projectile.h"
#include "zombie.h"


//- - - -


#define PI 3.14159265359
#define DEG2RAD(degree) (degree * 0.0174532925)

int windowWidth = 1200;
int windowHeight = 800;
bool keys[256] = {false};  // Keyboard Presses Tracker
bool isMouseLocked = true;

std::vector<Projectile> projectiles; // Active Projectiles
ProjectileType currentProjectileType = ProjectileType::BASIC;

std::vector<PowerUp> powerUps;
const float powerUpDuration = 10.0f; // Seconds
float powerUpTimer = 0.0f;

  std::vector<Zombie> zombies;
float zombieSpawnInterval = 5.0f;  // Spawning Cooldown
float zombieSpawnTimer = 0.0f;

Player player;
int score = 0;
float moveSpeed = 0.3f;

int level = 0; // Stadium, Circus
bool fadeOut = false;
bool fadeIn = false;
float fadeAlpha = 1.0f;
const float fadeDuration = 5.0f;
float fadeTimer = 0.0f; // Fade Effect Duration

Boss boss;

Cam camera;
bool isThirdPerson = true;   // Third-Person/First-Person Toggle
float rotAng = 0.0f;


//- - - -


void renderScore() {
    glColor3f(1.0f, 1.0f, 1.0f); // White text
    glRasterPos2f(-0.95f, 0.9f); // Position on screen
    std::string scoreText = "Score: " + std::to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}



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



void Anim() {
    rotAng += 0.01f; // Increment Rotation

    if (fadeOut || fadeIn) {
        fadeTimer += 0.001f;
        // std::cout << "fadeTimer: " << fadeTimer << std::endl;
    }
    
    // Level Transition Logic
    if (score >= 5 && level == 0) {
        zombies.clear(); // Clear Existing Zombies
        fadeOut = true;
        level = 1;
    }

    if (fadeOut && (fadeTimer >= fadeDuration / 2)) {
        fadeOut = false;
        fadeIn = true;
    }

    if (fadeIn && (fadeTimer >= fadeDuration)) {
        fadeIn = false;
        fadeTimer = 0;
    }

    if (fadeIn || fadeOut) {

        if (fadeOut) {
            fadeAlpha = 2 * (1 - fadeTimer / fadeDuration) - 1; // Decrease
        }
        else {
            fadeAlpha = 2 * (fadeTimer / fadeDuration) - 1; // Increase
        }
        //std::cout << "fadeAlpha: " << fadeAlpha << std::endl;
        if (fadeAlpha <= 0.03f) {
            GLfloat globalAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
            GLfloat black[] = { fadeAlpha, fadeAlpha, fadeAlpha, 1.0f };  // Completely dark
            glLightfv(GL_LIGHT0, GL_DIFFUSE, black);   // No diffuse light
            glLightfv(GL_LIGHT0, GL_AMBIENT, black);   // No ambient light
            glDisable(GL_LIGHT0);                      // Disable light source completely
        }
        else {
            GLfloat globalAmbient[] = { 0, 0, 0, 1.0f };
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
            GLfloat lightIntensity[] = { fadeAlpha, fadeAlpha, fadeAlpha, 1.0f };  // Modify light intensity
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);  // Modify Diffuse Light
            glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);  // Modify Ambient Light
            glEnable(GL_LIGHT0);  // Ensure the light is enabled
        }
    }
    

    // Update Projectiles
    for (size_t i = 0; i < projectiles.size(); ) {
        for (auto& projectile : projectiles) {
            projectile.update(zombies); // Pass The Zombies List For Homing Logic
        }

        // Remove Out-Of-Bounds Projectiles
        if (projectiles[i].isOutOfBounds()) {
            projectiles.erase(projectiles.begin() + i);
        }
        else {
            ++i;
        }
    }

    for (size_t i = 0; i < powerUps.size(); ) {
        if (powerUps[i].isCollected(player.position)) {
            currentProjectileType = powerUps[i].type; // Update Tthe Projectile Type
            powerUps.erase(powerUps.begin() + i);    // Remove The Collected Power-up
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

    if (level == 0) {
        // Update Zombies
        for (auto& zombie : zombies) {
            zombie.moveTowards(player.position);
        }

        // Spawn New Zombies
        zombieSpawnTimer += 0.01f;
        if (zombieSpawnTimer >= zombieSpawnInterval) {
            spawnZombie();
            zombieSpawnTimer = 0.0f;
        }
    }

    if (level == 1 && !(fadeOut || fadeIn)) {
        boss.moveTowards(player.position);
    }

    // Check For Collisions Between Projectiles And Zombies
    for (size_t i = 0; i < projectiles.size(); ) {
        bool projectileHit = false;

        for (size_t j = 0; j < zombies.size(); ) {
            if (projectiles[i].position.distance(zombies[j].position) < 1.0f) { // Collision Threshold
                zombies[j].takeDamage(projectiles[i].damage);

                if (zombies[j].isDead()) {
                    zombies.erase(zombies.begin() + j);
                    // Increase Score
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

        if (level == 1 && projectiles[i].position.distance(boss.position) < 1.0f) { // Collision Threshold
            boss.takeDamage(projectiles[i].damage);

            if (boss.isDead()) {
                exit(500); // Winning Logic
            }

            projectileHit = true;
            break;
        }

        if (projectileHit) {
            projectiles.erase(projectiles.begin() + i);
        }
        else {
            ++i;
        }
    }

    glutPostRedisplay(); // Request Display Update
}



void Display() {
    // Clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (keys['w'] || keys['W']) { // Move Forward
        player.moveForward(moveSpeed);
        std::cout << "Player Position: (" << player.position.x << ", "
            << player.position.y << ", " << player.position.z << ")\n";

    }
    if (keys['s'] || keys['S']) { // Move Backward
        player.moveForward(-moveSpeed);
        std::cout << "Player Position: (" << player.position.x << ", "
            << player.position.y << ", " << player.position.z << ")\n";

    }
    if (keys['a'] || keys['A']) { // Rotate Left
        player.moveRight(moveSpeed);
        std::cout << "Player Position: (" << player.position.x << ", "
            << player.position.y << ", " << player.position.z << ")\n";

    }
    if (keys['d'] || keys['D']) { // Rotate Right
        player.moveRight(-moveSpeed);
        std::cout << "Player Position: (" << player.position.x << ", "
            << player.position.y << ", " << player.position.z << ")\n";

    }

    renderScore();
    renderExplosions();

    // Render Projectiles
    for (const auto& projectile : projectiles) {
        projectile.render();
    }
    
    // Render Power-Ups
    for (const auto& powerUp : powerUps) {
        powerUp.render();
    }
    
    // Render Zombies
    for (const auto& zombie : zombies) {
        zombie.render();
    }

    if (level == 1 && !fadeOut) {
        // Render Boss
        boss.render();
    }

    // Update Camera Position & Orientation
    camera.followPlayer(isThirdPerson);
    cam();

    // Draw The Ground Plane
    glPushMatrix();
    glColor3f(0.8f, 0.8f, 0.8f); // Light Gray
    drawPlane(50.0f, 50);        // Plane Size 50x50, Divided Into 50 Segments
    glPopMatrix();

    // Render The Loaded 3D Model
    glPushMatrix();
    glTranslatef(player.position.x, player.position.y, player.position.z);
    glRotatef(player.rotationAngle, 0, -1, 0); // Rotate Based On Player's Rotation
    renderModel(); // Call The RenderModel Function From model_loader.cpp
    glPopMatrix();

    // Swap Buffers (For Double Buffering)
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

    // Display Callbacks
    glutDisplayFunc(Display);
    glutIdleFunc(Anim); // Called When System's Idel
    glutReshapeFunc(Reshape);
    glutPassiveMotionFunc(MouseMovement);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);

    // OpenGL Setup
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D); // Enable 2D textures

    // 3D Model Loading
    loadModel("assets/penguin_skipper.glb");
    spawnPowerUps();

    // Cursor Setup
    centerMouse();
    glutSetCursor(GLUT_CURSOR_NONE); // Hide the cursor

    // GLUT Event-Processing Loop
    glutMainLoop();

    return 0;
}