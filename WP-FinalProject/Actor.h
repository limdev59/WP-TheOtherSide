#pragma once
#include <vector>
#include <atlimage.h> // CImage 사용을 위해 포함

class Actor {
public:
    Actor(int x, int y);
    virtual ~Actor();

    void setPosition(int x, int y);
    POINT getPosition() const;

    void setHitbox(int width, int height);
    RECT getHitbox() const;

    void setImage(const CImage& image);
    const CImage& getImage() const;

    void addAnimationFrame(const CImage& image, int duration);
    const std::vector<std::pair<CImage, int>>& getAnimationFrames() const;

    virtual void update(int deltaTime);
    virtual void render(HDC hdc);

protected:
    POINT position;
    RECT hitbox;
    CImage currentImage;
    std::vector<std::pair<CImage, int>> animationFrames; // (이미지, 지속 시간)의 벡터
    int currentFrameIndex;
    int frameTime; // 현재 프레임이 얼마나 지속되었는지 추적
};
