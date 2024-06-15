#include "Animation.h"
#include <cmath>

Animation::Animation(const std::string& id, bool loop, float animationLength,
    const std::map<float, POINT>& positions,
    const std::map<float, POINT>& scales,
    const std::map<float, std::string>& images)
    : id(id), loop(loop), animationLength(animationLength), elapsedTime(0.0f),
    positions(positions), scales(scales), images(images), currentFrame(nullptr), currentFrameKey(0.0f) {

    for (const auto& [time, imageName] : images) {
        CImage* img = new CImage();
        std::string myStr = imageName + ".png";
        std::wstring myWideString(myStr.begin(), myStr.end());
        LPCTSTR lpctstr = myWideString.c_str();
        if (FAILED(img->Load(lpctstr))) {
            MessageBox(NULL, TEXT("Failed to load image"), TEXT("Error"), MB_OK);
            delete img;
        }
        else {
            loadedImages[imageName] = img;
        }
    }

    updateFrame(0.0f);
}

Animation::~Animation() {
    loadedImages.clear();
}

void Animation::update(float deltaTime) {
    elapsedTime += deltaTime;

    if (loop) {
        if (elapsedTime >= animationLength) {
            elapsedTime -= animationLength;
        }
    }
    else {
        if (elapsedTime >= animationLength) {
            elapsedTime = animationLength;
        }
    }

    updateFrame(elapsedTime);
}


const CImage* Animation::getCurrentFrame() const {
    return currentFrame;
}

float Animation::getCurrentFrameKey() const {
    return currentFrameKey;
}

std::string Animation::getId() const {
    return id;
}

bool Animation::isComplete() const {
    return elapsedTime >= animationLength && !loop;
}

void Animation::reset() {
    elapsedTime = 0.0f;
    updateFrame(elapsedTime);
}

void Animation::updateFrame(float time) {
    auto it = images.lower_bound(time);
    if (it != images.begin() && (it == images.end() || it->first > time)) {
        --it;
    }
    if (it != images.end()) {
        currentFrameKey = it->first;
        currentFrame = loadedImages[it->second];
    }
}