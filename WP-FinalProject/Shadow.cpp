#include "Shadow.h"

Shadow::Shadow(Vector3 position, Vector3 size)
    : Actor(position, size), isLeftDown(false), isRightDown(false), chargeLevel(0), maxChargeLevel(50), chargeRate(2), isHoldingObject(false), canCharge(true) {}

void Shadow::setLeftDown(bool state) {
    isLeftDown = state;
}

void Shadow::setRightDown(bool state) {
    isRightDown = state;
}

void Shadow::setDirection(Vector3 dir)
{
    direction = dir;
}

void Shadow::update(float deltaTime, POINT pos) {
    mousePos = pos;
    if (isLeftDown) {
        chargeLevel += chargeRate * deltaTime;
        if (chargeLevel > maxChargeLevel) {
            chargeLevel = maxChargeLevel;
        }
    }
    else if (chargeLevel > 0) {
        releaseCharge();
    }
}

void Shadow::releaseCharge()
{
    if (isHoldingObject) {
    }
    else {
        float distance = (chargeLevel / maxChargeLevel) * 10; // 최대 거리는 100으로 가정
        moveInDirection(distance);
    }
    chargeLevel = 0;
}

void Shadow::moveInDirection(float distance) {
    setPosition(getPosition() + direction * distance);
}