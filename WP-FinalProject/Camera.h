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

    Vector3 getDirection() const; // 새롭게 추가된 메서드

    void move(const Vector3& delta);
    void rotate(float yawDelta, float pitchDelta, float rollDelta);

private:
    Vector3 position;
    float yaw;
    float pitch;
    float roll;
};

bool Project3DTo2D(const Camera& cam, const Vector3& pos, POINT& point);