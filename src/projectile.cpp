#include "projectile.h"
#include <glut.h>
#include <chrono> // For timing


std::vector<std::pair<Vector3f, float>> Projectile::explosions;


Projectile::Projectile(Vector3f startPosition, Vector3f targetDirection, float speed, float damage, ProjectileType type)
    : position(startPosition), direction(targetDirection.normalize()), speed(speed), damage(damage), type(type) {}


void Projectile::update(const std::vector<Zombie>& zombies) {
    if (type == ProjectileType::HOMING) {
        // Find the nearest zombie
        const Zombie* nearestZombie = nullptr;
        float minDistance = std::numeric_limits<float>::max();

        for (const auto& zombie : zombies) {
            float dist = position.distance(zombie.position);
            if (dist < minDistance) {
                minDistance = dist;
                nearestZombie = &zombie;
            }
        }

        // Adjust direction to target the nearest zombie
        if (nearestZombie) {
            direction = (nearestZombie->position - position).normalize();
        }
    }

    // Update position based on current direction
    position = position + direction * speed;
}


void Projectile::render() const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color for the projectile
    glutSolidCube(0.2f);         // Render a smaller cube for the projectile
    glPopMatrix();
}

bool Projectile::isOutOfBounds() const {
    return position.x < -50.0f || position.x > 50.0f ||
        position.y < -10.0f || position.y > 50.0f ||
        position.z < -50.0f || position.z > 50.0f;


}




void Projectile::handleCollision(std::vector<Zombie>& zombies) {
    switch (type) {
    case ProjectileType::BASIC:
        // Standard damage to one zombie
        break;

    case ProjectileType::SPREAD:
        // Damage multiple zombies in a small radius
        for (auto& zombie : zombies) {
            if (position.distance(zombie.position) < 2.0f) { // Spread radius
                zombie.takeDamage(damage);
            }
        }
        break;

    case ProjectileType::EXPLOSIVE:
        // Damage all zombies in a larger radius
        for (auto& zombie : zombies) {
            if (position.distance(zombie.position) < 5.0f) { // Explosion radius
                zombie.takeDamage(damage * 2); // Increased damage
            }
        }

        explosions.emplace_back(position, 0.5f); // Explosion lasts 0.5 seconds
        break;

    case ProjectileType::HOMING:
        // Adjust direction to target the nearest zombie
        if (!zombies.empty()) {
            Zombie* nearest = nullptr;
            float minDistance = std::numeric_limits<float>::max();
            for (auto& zombie : zombies) {
                float dist = position.distance(zombie.position);
                if (dist < minDistance) {
                    minDistance = dist;
                    nearest = &zombie;
                }
            }
            if (nearest) {
                direction = (nearest->position - position).normalize();
            }
        }
        break;

    default:
        break;
    }
}
