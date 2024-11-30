#include "player.h"
#include <cmath>
#define DEG2RAD(a) (a * 0.0174532925)

Player::Player() : position(0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, -1.0f), rotationAngle(0.0f) {}

void Player::moveForward(float distance) {
    float rad = DEG2RAD(rotationAngle);
    position.x += cos(rad) * distance;
    position.z += sin(rad) * distance;
}

void Player::moveRight(float distance) {
    float rad = DEG2RAD(rotationAngle);
    Vector3f right(sin(rad), 0, -cos(rad)); // Perpendicular to the forward direction
    position = position + right * distance;
}


void Player::rotate(float angle) {
    rotationAngle += angle;
    while (rotationAngle > 360.0f) rotationAngle -= 360.0f;
    while (rotationAngle < 0.0f) rotationAngle += 360.0f;
}
