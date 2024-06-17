#pragma once

#include <cmath>

class Vector3 {
public:
    float x, y, z;

    // Default constructor
    Vector3() : x(0), y(0), z(0) {}

    // Parameterized constructor
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Calculate the length (magnitude) of the vector
    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // 벡터의 정규화 (단위 벡터화)
    Vector3 Normalized() const {
        float length = Length();
        if (length != 0)
            return Vector3(x / length, y / length, z / length);
        else
            return Vector3(0, 0, 0);
    }

    // Calculate the dot product of this vector and another vector
    float Dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Calculate the cross product of this vector and another vector
    Vector3 Cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Scalar multiplication
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    // Scalar division
    Vector3 operator/(float scalar) const {
        if (scalar != 0)
            return Vector3(x / scalar, y / scalar, z / scalar);
        else
            return Vector3(0, 0, 0); // Return zero vector if scalar is zero
    }

    // Assignment operator
    Vector3& operator=(const Vector3& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }

    // Vector addition
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    // Vector subtraction
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    // Additional utility methods (optional)

    // Calculate the distance to another vector
    float Distance(const Vector3& other) const {
        return (*this - other).Length();
    }

    // Project this vector onto another vector
    Vector3 ProjectOnto(const Vector3& other) const {
        Vector3 normalizedOther = other.Normalized();
        return normalizedOther * this->Dot(normalizedOther);
    }

    // Reflect this vector around another vector
    Vector3 Reflect(const Vector3& normal) const {
        return *this - normal * 2.0f * this->Dot(normal);
    }
};