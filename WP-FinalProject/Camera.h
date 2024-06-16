#pragma once
#include <Windows.h>
#include "Vector3.h"

class Camera {
public:
    Camera();
    Camera(Vector3 position, float yaw, float pitch, float roll);

    Vector3 getPosition() const;
    void setPosition(Vector3 pos);
    float getYaw() const;
    float getPitch() const;
    float getRoll() const;

    Vector3 getDirection() const;
    void move(const Vector3& delta);

    Vector3 getRotation() const;
    void setRotation(Vector3 rot);
    void rotate(float yawDelta, float pitchDelta, float rollDelta);

private:
    Vector3 position;
    float yaw;
    float pitch;
    float roll;
};

bool Project3DTo2D(const Camera& cam, const Vector3& pos, POINT& point);
Vector3 Unproject2DTo3D(const Camera& cam, const POINT& point, float y);