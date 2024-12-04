#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "vector3f.h"
#include "zombie.h"
#include <vector>
#include <utility> // For std::pair

enum class ProjectileType {
    BASIC,
    SPREAD,
    RAPID,
    EXPLOSIVE,
    HOMING
};

class Projectile {
public:
    Vector3f position;
    Vector3f direction;
    float speed;
    float damage;
    ProjectileType type;

    Projectile(Vector3f startPosition, Vector3f targetDirection, float speed, float damage, ProjectileType type);

    void update(const std::vector<Zombie>& zombies); // Pass zombies for homing logic

    void render() const;
    bool isOutOfBounds() const;


    static std::vector<std::pair<Vector3f, float>> explosions; // Explosion dat

    // Additional logic for special projectiles
    void handleCollision(std::vector<Zombie>& zombies);
};

#endif
