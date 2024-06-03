#pragma once

#include "Object3D.h"
#include "Animation.h"
#include <atlimage.h>
#include <vector>

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

    // 애니메이션 설정 및 업데이트
    void setImage(const CImage& image);
    const CImage& getImage() const;
    void addAnimationFrame(const CImage& image, int duration);
    void update(int deltaTime);
    void render(HDC hdc, const Camera& cam);

    // 3D 객체 그리기 메서드 오버라이드
    void DrawObject3D(HDC hdc, const Camera& cam, CImage& image);

private:
    POINT position2D; // 2D 위치
    RECT hitbox; // 히트박스
    Animation animation;
};
