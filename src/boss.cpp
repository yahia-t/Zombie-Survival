#include "boss.h"
#include <glut.h>
#include <cmath>

Boss::Boss() : position(0.0f, 0.0f, 0.0f), speed(0.01f), health(500.0f), alive(false) {}

void Boss::moveTowards(const Vector3f& targetPosition) {
    Vector3f direction = targetPosition - position;
    float distance = direction.magnitude();
    if (distance > 0.1f) {  // Only Move If Not Very Close
        direction = direction.normalize();
        position = position + direction * speed;
    }
}

void Boss::render() const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glColor3f(0.0f, 0.0f, 0.0f); // Black Color For The Boss
    glutSolidCube(5.0f);         // Render A 5x5x5 Cube
    glPopMatrix();
}

void Boss::takeDamage(float damage) {
    health -= damage;
}

bool Boss::isDead() const {
    return health <= 0;
}
