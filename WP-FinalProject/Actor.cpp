#include "Actor.h"

Actor::Actor(int x, int y) : position({ x, y }), currentFrameIndex(0), frameTime(0) {
    // �ʱ� ��Ʈ�ڽ� ũ�� ����
    setHitbox(50, 50);
}

Actor::~Actor() {
    // ���ҽ� ������ CImage�� �ڵ����� ó����
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
    // �ִϸ��̼� ������Ʈ ����
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
    // ���� �̹����� ȭ�鿡 �׸���
    if (currentImage) {
        currentImage.Draw(hdc, position.x, position.y);
    }
}
