#include "Actor.h"

Actor::Actor(int x, int y) : position({ x, y }), currentFrameIndex(0), frameTime(0) {
    // 초기 히트박스 크기 설정
    setHitbox(50, 50);
}

Actor::~Actor() {
    // 리소스 해제는 CImage가 자동으로 처리함
}

void Actor::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
}

POINT Actor::getPosition() const {
    return position;
}

void Actor::setHitbox(int width, int height) {
    hitbox = { position.x, position.y, position.x + width, position.y + height };
}

RECT Actor::getHitbox() const {
    return hitbox;
}

void Actor::setImage(const CImage& image) {
    currentImage = image;
}

const CImage& Actor::getImage() const {
    return currentImage;
}

void Actor::addAnimationFrame(const CImage& image, int duration) {
    animationFrames.push_back(std::make_pair(image, duration));
}

const std::vector<std::pair<CImage, int>>& Actor::getAnimationFrames() const {
    return animationFrames;
}

void Actor::update(int deltaTime) {
    // 애니메이션 업데이트 로직
    if (!animationFrames.empty()) {
        frameTime += deltaTime;
        if (frameTime >= animationFrames[currentFrameIndex].second) {
            frameTime = 0;
            currentFrameIndex = (currentFrameIndex + 1) % animationFrames.size();
            currentImage = animationFrames[currentFrameIndex].first;
        }
    }
}

void Actor::render(HDC hdc) {
    // 현재 이미지를 화면에 그리기
    if (currentImage) {
        currentImage.Draw(hdc, position.x, position.y);
    }
}
