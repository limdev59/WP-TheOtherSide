#include "Player.h"

Player::Player(Vector3 position, Vector3 size)
    : Actor(position, size), acceleration(0.0f), direction(Vector3(0.0f, 0.0f, 0.0f)) {
    // 초기화 코드
}

Player::~Player() {
    // 소멸자 코드 (필요한 경우)
}

void Player::setAcceleration(float acceleration) {
    this->acceleration = acceleration;
}

float Player::getAcceleration() const {
    return acceleration;
}

void Player::setDirection(Vector3 direction) {
    this->direction = direction;
}

Vector3 Player::getDirection() const {
    return direction;
}
