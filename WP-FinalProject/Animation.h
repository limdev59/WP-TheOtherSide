#pragma once

#include <atlimage.h>
#include <vector>

class Animation {
public:
    Animation();
    ~Animation();

    void addFrame(const CImage& image, int duration);
    void update(int deltaTime);
    const CImage& getCurrentFrame() const;

    std::vector<std::pair<CImage, int>> frames; // (이미지, 지속 시간)의 벡터
private:
    int currentFrameIndex;
    int frameTime; // 현재 프레임이 얼마나 지속되었는지 추적
};

