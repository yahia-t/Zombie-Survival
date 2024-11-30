#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <cmath>

class Vector3f {
public:
    float x, y, z;

    // Constructor
    Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
        : x(_x), y(_y), z(_z) {}

    // Vector addition
    Vector3f operator+(const Vector3f& v) const {
        return Vector3f(x + v.x, y + v.y, z + v.z);
    }

    // Vector subtraction
    Vector3f operator-(const Vector3f& v) const {
        return Vector3f(x - v.x, y - v.y, z - v.z);
    }

    // Scalar multiplication
    Vector3f operator*(float n) const {
        return Vector3f(x * n, y * n, z * n);
    }

    // Scalar division
    Vector3f operator/(float n) const {
        return (n != 0) ? Vector3f(x / n, y / n, z / n) : Vector3f(0, 0, 0);
    }

    // Normalize the vector
    Vector3f unit() const {
        float magnitude = std::sqrt(x * x + y * y + z * z);
        return (magnitude != 0) ? *this / magnitude : Vector3f(0, 0, 0);
    }

    // Dot product
    float dot(const Vector3f& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    // Cross product
    Vector3f cross(const Vector3f& v) const {
        return Vector3f(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }
};

#endif // VECTOR3F_H
