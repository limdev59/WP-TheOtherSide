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

    std::vector<std::pair<CImage, int>> frames; // (�̹���, ���� �ð�)�� ����
private:
    int currentFrameIndex;
    int frameTime; // ���� �������� �󸶳� ���ӵǾ����� ����
};

