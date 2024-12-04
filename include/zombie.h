#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "vector3f.h"

class Zombie {
public:
    Vector3f position;
    float speed;
    float health;

    Zombie(Vector3f spawnPosition, float speed = 0.02f, float health = 100.0f);
    void moveTowards(const Vector3f& targetPosition);
    void render() const; // Mark as const since it doesn't modify the zombie
    bool isDead() const;

    void takeDamage(float damage);

};

#endif
