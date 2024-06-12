#include "Player.h"

Player::Player(Vector3 position, Vector3 size)
    : Actor(position, size), acceleration(0.0f), direction(Vector3(0.0f, 0.0f, 0.0f)) {
    // �ʱ�ȭ �ڵ�
}

Player::~Player() {
    // �Ҹ��� �ڵ� (�ʿ��� ���)
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
