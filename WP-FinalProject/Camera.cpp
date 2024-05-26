#include "Camera.h"
#include "Vector3.h"
#include <cmath>
#include <Windows.h>

Camera::Camera() : position{ 0, 0, 0 }, yaw(0), pitch(0) {}

Camera::Camera(Vector3 position, float yaw, float pitch) : position(position), yaw(yaw), pitch(pitch) {}

Vector3 Camera::getPosition() const {
    return position;
}

float Camera::getYaw() const {
    return yaw;
}

float Camera::getPitch() const {
    return pitch;
}

void Camera::move(const Vector3& delta) {
    position.x += delta.x;
    position.y += delta.y;
    position.z += delta.z;
}

void Camera::rotate(float yawDelta, float pitchDelta) {
    yaw += yawDelta;
    pitch += pitchDelta;
}

bool Project3DTo2D(const Camera& cam, const Vector3& pos, POINT& point) {
    constexpr float NEAR_PLANE = 0.1f;
    constexpr float FOV = 90.0f;
    constexpr float ASPECT_RATIO = 800.0f / 600.0f;
    float cosYaw = cosf(cam.getYaw());
    float sinYaw = sinf(cam.getYaw());
    float cosPitch = cosf(cam.getPitch());
    float sinPitch = sinf(cam.getPitch());

    float x = pos.x - cam.getPosition().x;
    float y = pos.y - cam.getPosition().y;
    float z = pos.z - cam.getPosition().z;

    float dx = cosYaw * x - sinYaw * z;
    float dz = sinYaw * x + cosYaw * z;

    float dy = cosPitch * y - sinPitch * dz;
    dz = sinPitch * y + cosPitch * dz;

    if (dz <= NEAR_PLANE) return false;

    float fovTan = tanf(FOV * 0.5f * (3.14159265358979323846f / 180.0f));
    point.x = (LONG)((dx / (dz * fovTan * ASPECT_RATIO)) * 600 + 770);
    point.y = (LONG)(450 - (dy / (dz * fovTan)) * 450);

    return true;
}
