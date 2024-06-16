#pragma once
#include "Actor.h"
#include "Vector3.h"
#include <Windows.h>

class Shadow : public Actor {
public:
    Shadow(Vector3 position, Vector3 size);

    void setLeftDown(bool state);
    void setRightDown(bool state);
    void setDirection(Vector3 direction);
    void update(float deltaTime, POINT pos, Vector3 playerPosition);
    void onMouseMove(LPARAM lParam);

    Vector3 getDirection() const;
    Vector3 getPosition() const;

private:
    Vector3 direction;
    POINT mousePos;
    bool isLeftDown;
    bool isRightDown;
    bool isHoldingObject;
    bool canCharge;

    float chargeLevel;
    float maxChargeLevel;
    float chargeRate;

    void releaseCharge(Vector3 playerPosition);
    void moveInDirection(float distance, Vector3 playerPosition);
    void throwObject(Vector3 direction);
};
