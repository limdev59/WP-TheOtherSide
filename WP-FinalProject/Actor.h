#pragma once

#include "Object3D.h"
#include "Animation.h"
#include <atlimage.h>
#include <vector>

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

    // �ִϸ��̼� ���� �� ������Ʈ
    void setImage(const CImage& image);
    const CImage& getImage() const;
    void addAnimationFrame(const CImage& image, int duration);
    void update(int deltaTime);
    void render(HDC hdc, const Camera& cam);

    // 3D ��ü �׸��� �޼��� �������̵�
    void DrawObject3D(HDC hdc, const Camera& cam, CImage& image);

private:
    POINT position2D; // 2D ��ġ
    RECT hitbox; // ��Ʈ�ڽ�
    Animation animation;
};
