#include <iostream>
#include <glew.h>
#include <glut.h>
#include "camera.h"
#include "player.h"
#include "model_loader.h"
#include "main.h"

Player player;
Cam camera;


float rotAng = 0.0f;
bool keys[256] = { false };  // Key state tracking
bool isMouseLocked = true;   // Flag to lock the mouse inside
bool isThirdPerson = true;   // Third-person/first-person toggle
int windowWidth = 1200;
int windowHeight = 800;
float moveSpeed = 0.3f;

// Implementations of functions declared in main.h



// Function prototypes
void Anim() {
    rotAng += 0.01; // Increment rotation angle
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



    // Center mouse
    centerMouse();
    glutSetCursor(GLUT_CURSOR_NONE); // Hide the cursor

    // Enter the GLUT event-processing loop
    glutMainLoop();

    return 0;
}

