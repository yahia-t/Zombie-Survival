#ifndef CAMERA_H
#define CAMERA_H

#include "vector3f.h"
#include "player.h"

class Cam {
public:
    Vector3f eye, center, top;

    Cam();
    void followPlayer(bool isThirdPerson);
    void look();
};

// Standalone functions
void drawPlane(float size, int divisions);
void cam();
void centerMouse();

#endif