#include "Camera.h"
#include "Vector3.h"
#include <cmath>
#include <Windows.h>

Camera::Camera() : position{ 0, 0, 0 }, yaw(0), pitch(0), roll(0) {}

Camera::Camera(Vector3 position, float yaw, float pitch, float roll)
    : position(position), yaw(yaw), pitch(pitch), roll(roll) {}

Vector3 Camera::getPosition() const {
    return position;
}

void Camera::setPosition(Vector3 pos)
{
    position = pos;
}

float Camera::getYaw() const {
    return yaw;
}

float Camera::getPitch() const {
    return pitch;
}

float Camera::getRoll() const {
    return roll;
}

Vector3 Camera::getRotation() const
{
    return Vector3(yaw, pitch, roll);
}

void Camera::setRotation(Vector3 rot) {
    yaw = rot.x;
    pitch = rot.y;
    roll = rot.z;
}

Vector3 Camera::getDirection() const {
    // 카메라의 방향 벡터 계산
    float cosYaw = cosf(yaw);
    float sinYaw = sinf(yaw);
    float cosPitch = cosf(pitch);
    float sinPitch = sinf(pitch);

    return Vector3(
        cosYaw * cosPitch,
        sinPitch,
        sinYaw * cosPitch
    ).Normalize();
}

void Camera::move(const Vector3& delta) {
    position.x += delta.x;
    position.y += delta.y;
    position.z += delta.z;
}

void Camera::rotate(float yawDelta, float pitchDelta, float rollDelta) {
    yaw += yawDelta;
    pitch += pitchDelta;
    roll += rollDelta;
}

bool Project3DTo2D(const Camera& cam, const Vector3& pos, POINT& point) {
    constexpr float NEAR_PLANE = 0.1f;
    constexpr float FOV = 90.0f;
    constexpr float ASPECT_RATIO = 800.0f / 600.0f;

    float cosYaw = cosf(cam.getYaw());
    float sinYaw = sinf(cam.getYaw());
    float cosPitch = cosf(cam.getPitch());
    float sinPitch = sinf(cam.getPitch());
    float cosRoll = cosf(cam.getRoll());
    float sinRoll = sinf(cam.getRoll());

    Vector3 camPos = cam.getPosition(); // 카메라 위치를 한 번만 가져옴

    float x = pos.x - camPos.x;
    float y = pos.y - camPos.y;
    float z = pos.z - camPos.z;

    float dx = cosYaw * x - sinYaw * z;
    float dz = sinYaw * x + cosYaw * z;

    float dy = cosPitch * y - sinPitch * dz;
    dz = sinPitch * y + cosPitch * dz;

    float finalX = dx * cosRoll - dy * sinRoll;
    float finalY = dx * sinRoll + dy * cosRoll;

    if (dz <= NEAR_PLANE) return false;

    float fovTan = tanf(FOV * 0.5f * (3.14159265358979323846f / 180.0f));
    point.x = (LONG)((finalX / (dz * fovTan * ASPECT_RATIO)) * 600 + 770);
    point.y = (LONG)(450 - (finalY / (dz * fovTan)) * 450);

    return true;
}