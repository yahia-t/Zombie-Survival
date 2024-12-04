#include "powerup.h"
#include <cmath>
#include <glut.h>

PowerUp::PowerUp(Vector3f spawnPosition, ProjectileType type)
    : position(spawnPosition), type(type) {}

void PowerUp::render() const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);

    // Color based on power-up type
    switch (type) {
    case ProjectileType::BASIC: glColor3f(1.0f, 1.0f, 1.0f); break; // White
    case ProjectileType::SPREAD: glColor3f(1.0f, 0.0f, 0.0f); break; // Red
    case ProjectileType::RAPID: glColor3f(0.0f, 1.0f, 0.0f); break; // Green
    case ProjectileType::EXPLOSIVE: glColor3f(0.0f, 0.0f, 1.0f); break; // Blue
    case ProjectileType::HOMING: glColor3f(1.0f, 1.0f, 0.0f); break; // Yellow
    }

    glutSolidCube(1.0f); // Render as a cube
    glPopMatrix();
}

bool PowerUp::isCollected(const Vector3f& playerPosition) const {
    return position.distance(playerPosition) < 1.5f; // Collection threshold
}
