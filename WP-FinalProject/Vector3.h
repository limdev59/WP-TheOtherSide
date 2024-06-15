#pragma once

#include <cmath>

class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // ������ ���� ���
    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // ������ ����ȭ (���� ����ȭ)
    Vector3 Normalize() const {
        float length = Length();
        if (length != 0)
            return Vector3(x / length, y / length, z / length);
        else
            return Vector3(0, 0, 0);
    }

    // ������ ���� ���
    float Dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // ������ ���� ���
    Vector3 Cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // ��Į�� �� ����
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator/(float scalar) const {
        if (scalar != 0)
            return Vector3(x / scalar, y / scalar, z / scalar);
        else
            return Vector3(0, 0, 0);
    }

    Vector3& operator=(const Vector3& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }

    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
};