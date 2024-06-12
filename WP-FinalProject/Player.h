#pragma once

#include "Actor.h"

class Player : public Actor {
public:
    Player(Vector3 position, Vector3 size);
    virtual ~Player();

    // ���ӵ� �� ���� ���� �� ��������
    void setAcceleration(float acceleration);
    float getAcceleration() const;

    void setDirection(Vector3 direction);
    Vector3 getDirection() const;

private:
    float acceleration; // ���ӵ�
    Vector3 direction;  // ����
};
