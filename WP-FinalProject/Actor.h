#pragma once
#include <vector>
#include <atlimage.h> // CImage ����� ���� ����

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
    std::vector<std::pair<CImage, int>> animationFrames; // (�̹���, ���� �ð�)�� ����
    int currentFrameIndex;
    int frameTime; // ���� �������� �󸶳� ���ӵǾ����� ����
};
