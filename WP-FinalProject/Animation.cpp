#include "Animation.h"

Animation::Animation() : currentFrameIndex(0), frameTime(0) {}

Animation::~Animation() {}

void Animation::addFrame(const CImage& image, int duration) {
    frames.push_back(std::make_pair(image, duration));
}

void Animation::update(int deltaTime) {
    if (frames.empty()) return;

    frameTime += deltaTime;
    if (frameTime >= frames[currentFrameIndex].second) {
        frameTime = 0;
        currentFrameIndex = (currentFrameIndex + 1) % frames.size();
    }
}

const CImage& Animation::getCurrentFrame() const {
    return frames[currentFrameIndex].first;
}
