#include "Player.h"

Player::Player(int x, int y) : Actor(x, y), speed(5) {
    // �÷��̾� �ʱ�ȭ (�ʿ��)
}

Player::~Player() {
    // ���ҽ� ������ CImage�� �ڵ����� ó����
}

void Player::handleInput(WPARAM wParam) {
    
}

void Player::update(int deltaTime) {
    // �÷��̾� ������Ʈ ���� (�ʿ��)
    Actor::update(deltaTime);
}

void Player::render(HDC hdc) {
    // �÷��̾� ������ ����
    Actor::render(hdc);
}
