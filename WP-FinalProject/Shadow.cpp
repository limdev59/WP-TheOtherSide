#include "Shadow.h"

Shadow::Shadow(Vector3 position, Vector3 size)
    : Actor(position, size), isLeftDown(false), isRightDown(false), chargeLevel(0), maxChargeLevel(50), chargeRate(2), isHoldingObject(false), canCharge(true) {}

void Shadow::setLeftDown(bool state) {
    isLeftDown = state;
    if (state) {
        chargeLevel = 0;
        canCharge = true;
    }
}

void Shadow::setRightDown(bool state) {
    isRightDown = state;
}

void Shadow::setDirection(Vector3 dir) {
    direction = dir;
}

Vector3 Shadow::getDirection() const {
    return direction;
}

Vector3 Shadow::getPosition() const {
    return Actor::getPosition();
}

void Shadow::onMouseMove(LPARAM lParam) {
    mousePos.x = LOWORD(lParam);
    mousePos.y = HIWORD(lParam);
}

void Shadow::update(float deltaTime, POINT pos, Vector3 playerPosition) {
    mousePos = pos;
    if (isLeftDown && canCharge) {
        chargeLevel += chargeRate * deltaTime;
        if (chargeLevel > maxChargeLevel) {
            chargeLevel = maxChargeLevel;
        }
    }

    // �׸��ڰ� �÷��̾���� �ִ� �Ÿ��� �����ϵ��� ����
    float distanceToPlayer = (getPosition() - playerPosition).Length();
    const float MAX_DISTANCE = 10.0f; // �÷��̾�κ����� �ִ� ��� �Ÿ�

    if (distanceToPlayer > MAX_DISTANCE) {
        Vector3 directionToPlayer = (playerPosition - getPosition()).Normalized();
        setPosition(playerPosition + directionToPlayer * MAX_DISTANCE);
    }
}

void Shadow::releaseCharge(Vector3 playerPosition) {
    float distance = (chargeLevel / maxChargeLevel) * 10; // �ִ� �Ÿ��� 10���� ����
    moveInDirection(distance, playerPosition);
    chargeLevel = 0;
}

void Shadow::moveInDirection(float distance, Vector3 playerPosition) {
    Vector3 newPosition = getPosition() + direction * distance;
    const float MAX_DISTANCE = 10.0f; // �ִ� �Ÿ� ����

    if ((newPosition - playerPosition).Length() > MAX_DISTANCE) {
        newPosition = playerPosition + (newPosition - playerPosition).Normalized() * MAX_DISTANCE;
    }

    setPosition(newPosition);
}

void Shadow::throwObject(Vector3 throwDirection) {
    // throwDirection �� chargeLevel�� ����� ������ ���� ����
    releaseCharge(getPosition());
}
