#ifndef PLAYER_H
#define PLAYER_H

#include "vector3f.h"

class Player {
public:
    Vector3f position;
    Vector3f direction;
    float rotationAngle;

    Player();
    void moveForward(float distance);
    void moveRight(float distance);
    void rotate(float angle);
};

#endif
