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

    // 2D ��ġ ���� �� �̵�
    void set2DPosition(int x, int y);
    POINT get2DPosition() const;
    void move2DPosition(int dx, int dy);

    // ��Ʈ�ڽ� ���� �� ��������
    void setHitbox(int width, int height);
    RECT getHitbox() const;

    // �ִϸ��̼� ��Ʈ�ѷ� ����
    void setAnimationController(const AnimationController& animationController);
    AnimationController& getAnimationController();

    // 3D ��ü �׸��� �޼��� �������̵�
    void DrawObject3D(HDC hdc, const Camera& cam) override;

private:
    POINT position2D; // 2D ��ġ
    RECT hitbox; // ��Ʈ�ڽ�
    AnimationController animationController; // �ִϸ��̼� ��Ʈ�ѷ�

    Bitmap* CreateTransformedBitmap(HDC hdc, const CImage* image, bool rotate90, bool flipHorizontal, bool flipVertical);
    HBITMAP CreateMask(HDC hdc, Bitmap* pTransformedBitmap, int frameWidth, int frameHeight, Camera cam);
};
