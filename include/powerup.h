#ifndef POWERUP_H
#define POWERUP_H

#include "vector3f.h"
#include "projectile.h" // Include to use ProjectileType
#include <glut.h>

class PowerUp {
public:
    Vector3f position;
    ProjectileType type; // Use ProjectileType instead of PowerUpType

    PowerUp(Vector3f spawnPosition, ProjectileType type);
    void render() const;
    bool isCollected(const Vector3f& playerPosition) const;
};

#endif
