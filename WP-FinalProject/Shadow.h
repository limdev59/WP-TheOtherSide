#pragma once
#include "Player.h"

class Shadow : public Actor {
public:
    Shadow(Vector3 position, Vector3 size);

    void setLeftDown(bool state);
    void setRightDown(bool state);
    void setDirection(Vector3 direction);
    void update(float deltaTime, POINT pos);

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

    void releaseCharge();
    void moveInDirection(float distance);
};