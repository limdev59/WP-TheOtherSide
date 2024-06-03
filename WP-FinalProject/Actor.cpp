// Actor.cpp
#include "Vector3.h"
#include "Actor.h"
#include <gdiplus.h>
using namespace Gdiplus;

Actor::Actor(Vector3 position, Vector3 size)
    : Object3D(position, size), position2D({ 0, 0 }), hitbox({ 0, 0, 0, 0 }) {
    // 초기화 코드
}

Actor::~Actor() {
    // 소멸자 코드 (필요한 경우)
}

void Actor::set2DPosition(int x, int y) {
    position2D.x = x;
    position2D.y = y;
}

POINT Actor::get2DPosition() const {
    return position2D;
}

void Actor::move2DPosition(int dx, int dy) {
    position2D.x += dx;
    position2D.y += dy;
    // 히트박스도 함께 이동시킴
    hitbox.left += dx;
    hitbox.right += dx;
    hitbox.top += dy;
    hitbox.bottom += dy;
}

void Actor::setHitbox(int width, int height) {
    hitbox.left = position2D.x;
    hitbox.top = position2D.y;
    hitbox.right = position2D.x + width;
    hitbox.bottom = position2D.y + height;
}

RECT Actor::getHitbox() const {
    return hitbox;
}

void Actor::setImage(const CImage& image) {
    animation.addFrame(image, 0);
}

const CImage& Actor::getImage() const {
    return animation.getCurrentFrame();
}

void Actor::addAnimationFrame(const CImage& image, int duration) {
    animation.addFrame(image, duration);
}

void Actor::update(int deltaTime) {
    animation.update(deltaTime);
}

void Actor::render(HDC hdc, const Camera& cam) {
    // 2D 위치를 사용하여 현재 프레임을 그리기
    animation.getCurrentFrame().Draw(hdc, position2D.x, position2D.y);
}

void Actor::DrawObject3D(HDC hdc, const Camera& cam, CImage& image) {
    // 원본 애니메이션 이미지의 크기
    int frameWidth = image.GetWidth();
    int frameHeight = image.GetHeight();

    if (frameWidth <= 0 || frameHeight <= 0) {
        return; // 유효하지 않은 프레임 크기
    }

    Vector3 center = getPosition();
    Vector3 size = getSize();
    Vector3 halfSize = size / 2.0f;

    Vector3 vertices[4];
    POINT points[4];
    bool allPointsProjected = true;

    // 부등변 사각형의 네 점을 정의
    if (size.y == 0) {
        // 바닥
        vertices[0] = { center.x - halfSize.x, center.y, center.z + halfSize.z };
        vertices[1] = { center.x + halfSize.x, center.y, center.z + halfSize.z };
        vertices[2] = { center.x - halfSize.x, center.y, center.z - halfSize.z };
        vertices[3] = { center.x + halfSize.x, center.y, center.z - halfSize.z };
    }
    else if (size.z == 0) {
        // 가로벽
        vertices[0] = { center.x - halfSize.x, center.y + halfSize.y, center.z };
        vertices[1] = { center.x + halfSize.x, center.y + halfSize.y, center.z };
        vertices[2] = { center.x - halfSize.x, center.y - halfSize.y, center.z };
        vertices[3] = { center.x + halfSize.x, center.y - halfSize.y, center.z };
    }
    else if (size.x == 0) {
        // 세로벽
        vertices[0] = { center.x, center.y - halfSize.y, center.z + halfSize.z };
        vertices[1] = { center.x, center.y + halfSize.y, center.z + halfSize.z };
        vertices[2] = { center.x, center.y - halfSize.y, center.z - halfSize.z };
        vertices[3] = { center.x, center.y + halfSize.y, center.z - halfSize.z };
    }
    else {
        return; // 유효한 면이 아닌 경우 함수 종료
    }

    // 점들을 2D로 투영
    for (int i = 0; i < 4; ++i) {
        if (!Project3DTo2D(cam, vertices[i], points[i])) {
            allPointsProjected = false;
        }
    }

    // 모든 점이 투영되지 않았다면 그리지 않음
    if (!allPointsProjected) {
        return;
    }

    // 메모리 DC 생성 및 원본 이미지를 메모리 DC에 복사
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, frameWidth, frameHeight);
    HBITMAP oldMemBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
    image.Draw(memDC, 0, 0);  // 이미지를 메모리 DC에 그림

    // 투명 색상을 위한 마스크 비트맵 생성
    HBITMAP maskBitmap = CreateBitmap(frameWidth, frameHeight, 1, 1, NULL);
    HDC maskDC = CreateCompatibleDC(hdc);
    HBITMAP oldMaskBitmap = (HBITMAP)SelectObject(maskDC, maskBitmap);

    // 투명색을 마스크로 설정 (검은색 배경을 투명하게)
    for (int y = 0; y < frameHeight; ++y) {
        for (int x = 0; x < frameWidth; ++x) {
            COLORREF color = GetPixel(memDC, x, y);
            SetPixel(maskDC, x, y, (color == RGB(0, 0, 0)) ? RGB(0, 0, 0) : RGB(255, 255, 255));
        }
    }

    // 변형된 이미지를 그리기
    PlgBlt(hdc, points, memDC, 0, 0, frameWidth, frameHeight, maskBitmap, 0, 0);

    // 클린업
    SelectObject(memDC, oldMemBitmap);
    SelectObject(maskDC, oldMaskBitmap);
    DeleteObject(maskBitmap);
    DeleteDC(maskDC);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
}
