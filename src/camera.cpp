#include "camera.h"
#include <glut.h>
#include <cmath>
#include "main.h"
#include <windows.h> 
#include "player.h"

#define DEG2RAD(a) (a * 0.0174532925)

// Initialize Cam
Cam::Cam() : eye(0, 0, 0), center(0, 0, 0), top(0, 1, 0) {}

void Cam::followPlayer(bool isThirdPerson) {
    if (isThirdPerson) {
        // Third-person camera logic
        float rad = DEG2RAD(player.rotationAngle);
        eye = player.position + Vector3f(-cos(rad) * 5.0f, 2.0f, -sin(rad) * 5.0f); // Behind and above
        center = player.position;
    }
    else {
        float rad = DEG2RAD(player.rotationAngle);
        eye = player.position + Vector3f(0.0f, 1.5f, 0.0f);
        center = player.position + Vector3f(cos(rad) * 5.0f, 0.0f, sin(rad) * 5.0f);
    }
}

void Cam::look() {
    gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, top.x, top.y, top.z);
}

void drawPlane(float size, int divisions) {
    float halfSize = size / 2.0f;
    float step = size / divisions;

    glColor3f(0.8f, 0.8f, 0.8f); // Light gray color for the plane
    glBegin(GL_QUADS);
    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            float x = -halfSize + i * step;
            float z = -halfSize + j * step;

            glVertex3f(x, 0, z);
            glVertex3f(x + step, 0, z);
            glVertex3f(x + step, 0, z + step);
            glVertex3f(x, 0, z + step);
        }
    }
    glEnd();
}

void cam() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.look();
}

void centerMouse() {
    POINT windowCenter;
    windowCenter.x = windowWidth / 2;
    windowCenter.y = windowHeight / 2;
    ClientToScreen(GetActiveWindow(), &windowCenter);
    SetCursorPos(windowCenter.x, windowCenter.y);
}