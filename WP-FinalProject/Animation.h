// Animation.h
#pragma once

#include <map>
#include <string>
#include <atlimage.h>

class Animation {
public:
    Animation() {}
    Animation(const std::string& id, bool loop, float animationLength,
        const std::map<float, POINT>& positions,
        const std::map<float, POINT>& scales,
        const std::map<float, std::string>& images);
    ~Animation();
    void update(float deltaTime);
    const CImage* getCurrentFrame() const;
    float getCurrentFrameKey() const;

    std::string getId() const;
    bool isComplete() const;

private:
    std::string id;
    bool loop;
    float animationLength;
    float elapsedTime;

    std::map<float, POINT> positions;
    std::map<float, POINT> scales;
    std::map<float, std::string> images;
    std::map<std::string, CImage*> loadedImages;

    float currentFrameKey;
    CImage* currentFrame;

    void updateFrame(float time);
};