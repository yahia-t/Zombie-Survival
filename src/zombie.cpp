#include "zombie.h"
#include <glut.h>
#include <cmath>

Zombie::Zombie(Vector3f spawnPosition, float speed, float health)
    : position(spawnPosition), speed(speed), health(health) {}

void Zombie::moveTowards(const Vector3f& targetPosition) {
    Vector3f direction = targetPosition - position;
    float distance = direction.magnitude();
    if (distance > 0.1f) {  // Only move if not very close
        direction = direction.normalize(); // Use your normalize method here
        position = position + direction * speed;
    }
}

void Zombie::render() const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for the zombie
    glutSolidCube(1.0f);         // Render a 1x1x1 cube
    glPopMatrix();
}


void Zombie::takeDamage(float damage) {
    health -= damage;
}


bool Zombie::isDead() const {
    return health <= 0;
}
