#ifndef BOSS_H
#define BOSS_H

#include "vector3f.h"

class Boss {
public:
    Vector3f position;
    float speed;
    float health;
    bool alive;

    Boss();

    void moveTowards(const Vector3f& targetPosition);
    void render() const; // Mark As Const Since It Doesn't Modify The Boss
    bool isDead() const;
    void takeDamage(float damage);
};

#endif
