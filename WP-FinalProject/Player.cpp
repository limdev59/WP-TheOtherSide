#include "Player.h"

Player::Player(int x, int y) : Actor(x, y), speed(5) {
    // 플레이어 초기화 (필요시)
}

Player::~Player() {
    // 리소스 해제는 CImage가 자동으로 처리함
}

void Player::handleInput(WPARAM wParam) {
    
}

void Player::update(int deltaTime) {
    // 플레이어 업데이트 로직 (필요시)
    Actor::update(deltaTime);
}

void Player::render(HDC hdc) {
    // 플레이어 렌더링 로직
    Actor::render(hdc);
}
