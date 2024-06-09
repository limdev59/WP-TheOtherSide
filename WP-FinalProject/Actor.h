//Actor.h
#pragma once

#include "Object3D.h"
#include "AnimationController.h"
#include <atlimage.h>
#include <gdiplus.h>

using namespace Gdiplus;

class Actor : public Object3D {
public:
    Actor(Vector3 position, Vector3 size);
    virtual ~Actor();

    // 2D 위치 설정 및 이동
    void set2DPosition(int x, int y);
    POINT get2DPosition() const;
    void move2DPosition(int dx, int dy);

    // 히트박스 설정 및 가져오기
    void setHitbox(int width, int height);
    RECT getHitbox() const;

    // 애니메이션 컨트롤러 설정
    void setAnimationController(const AnimationController& animationController);
    AnimationController& getAnimationController();

    // 3D 객체 그리기 메서드 오버라이드
    void DrawObject3D(HDC hdc, const Camera& cam) override;

private:
    POINT position2D; // 2D 위치
    RECT hitbox; // 히트박스
    AnimationController animationController; // 애니메이션 컨트롤러

    Bitmap* CreateTransformedBitmap(HDC hdc, const CImage* image, bool rotate90, bool flipHorizontal, bool flipVertical);
    HBITMAP CreateMask(HDC hdc, Bitmap* pTransformedBitmap, int frameWidth, int frameHeight, Camera cam);
};
