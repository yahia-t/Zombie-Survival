#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <cmath>
#include <iostream>
#include <stdlib.h>

#define M_PI 3.14159265358979323846

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

    // Magnitude of the vector
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Distance between this vector and another
    float distance(const Vector3f& v) const {
        return (*this - v).magnitude();
    }

    // Check if two vectors are equal (with epsilon for floating-point precision)
    bool equals(const Vector3f& v, float epsilon = 1e-5) const {
        return std::abs(x - v.x) < epsilon &&
            std::abs(y - v.y) < epsilon &&
            std::abs(z - v.z) < epsilon;
    }

    // Print the vector components
    void print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    // Angle between this vector and another (in degrees)
    float angleBetween(const Vector3f& v) const {
        float dotProduct = this->dot(v);
        float magnitudes = this->magnitude() * v.magnitude();
        return (magnitudes != 0) ? std::acos(dotProduct / magnitudes) * (180.0 / M_PI) : 0.0f;
    }

    Vector3f& normalize() {
        float magnitude = this->magnitude();
        if (magnitude != 0) {
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
        }
        return *this;
    }


    // Rotate the vector around the Y-axis by a given angle (in degrees)
    Vector3f rotateY(float angleDegrees) const {
        float angleRadians = angleDegrees * (M_PI / 180.0f); // Convert degrees to radians
        float cosAngle = cos(angleRadians);
        float sinAngle = sin(angleRadians);

        return Vector3f(
            x * cosAngle - z * sinAngle, // Rotated X component
            y,                           // Y component remains unchanged
            x * sinAngle + z * cosAngle  // Rotated Z component
        );
    }





};

#endif // VECTOR3F_H
