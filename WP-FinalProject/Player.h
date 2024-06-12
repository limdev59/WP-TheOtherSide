#pragma once

#include "Actor.h"

class Player : public Actor {
public:
    Player(Vector3 position, Vector3 size);
    virtual ~Player();

    // 가속도 및 방향 설정 및 가져오기
    void setAcceleration(float acceleration);
    float getAcceleration() const;

    void setDirection(Vector3 direction);
    Vector3 getDirection() const;

private:
    float acceleration; // 가속도
    Vector3 direction;  // 방향
};
