#pragma once
#include <Windows.h>
#include "Vector3.h"

class Camera {
public:
    Camera();
    Camera(Vector3 position, float yaw, float pitch);

    Vector3 getPosition() const;
    float getYaw() const;
    float getPitch() const;

    void move(const Vector3& delta);
    void rotate(float yawDelta, float pitchDelta);

private:
    Vector3 position;
    float yaw;
    float pitch;
};

bool Project3DTo2D(const Camera& cam, const Vector3& pos, POINT& point);
