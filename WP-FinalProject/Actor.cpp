// Actor.cpp
#include "Vector3.h"
#include "Actor.h"
#include <gdiplus.h>
using namespace Gdiplus;

Actor::Actor(Vector3 position, Vector3 size)
    : Object3D(position, size), position2D({ 0, 0 }), hitbox({ 0, 0, 0, 0 }) {
    // �ʱ�ȭ �ڵ�
}

Actor::~Actor() {
    // �Ҹ��� �ڵ� (�ʿ��� ���)
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
    // ��Ʈ�ڽ��� �Բ� �̵���Ŵ
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
    // 2D ��ġ�� ����Ͽ� ���� �������� �׸���
    animation.getCurrentFrame().Draw(hdc, position2D.x, position2D.y);
}

void Actor::DrawObject3D(HDC hdc, const Camera& cam, CImage& image) {
    // ���� �ִϸ��̼� �̹����� ũ��
    int frameWidth = image.GetWidth();
    int frameHeight = image.GetHeight();

    if (frameWidth <= 0 || frameHeight <= 0) {
        return; // ��ȿ���� ���� ������ ũ��
    }

    Vector3 center = getPosition();
    Vector3 size = getSize();
    Vector3 halfSize = size / 2.0f;

    Vector3 vertices[4];
    POINT points[4];
    bool allPointsProjected = true;

    // �ε �簢���� �� ���� ����
    if (size.y == 0) {
        // �ٴ�
        vertices[0] = { center.x - halfSize.x, center.y, center.z + halfSize.z };
        vertices[1] = { center.x + halfSize.x, center.y, center.z + halfSize.z };
        vertices[2] = { center.x - halfSize.x, center.y, center.z - halfSize.z };
        vertices[3] = { center.x + halfSize.x, center.y, center.z - halfSize.z };
    }
    else if (size.z == 0) {
        // ���κ�
        vertices[0] = { center.x - halfSize.x, center.y + halfSize.y, center.z };
        vertices[1] = { center.x + halfSize.x, center.y + halfSize.y, center.z };
        vertices[2] = { center.x - halfSize.x, center.y - halfSize.y, center.z };
        vertices[3] = { center.x + halfSize.x, center.y - halfSize.y, center.z };
    }
    else if (size.x == 0) {
        // ���κ�
        vertices[0] = { center.x, center.y - halfSize.y, center.z + halfSize.z };
        vertices[1] = { center.x, center.y + halfSize.y, center.z + halfSize.z };
        vertices[2] = { center.x, center.y - halfSize.y, center.z - halfSize.z };
        vertices[3] = { center.x, center.y + halfSize.y, center.z - halfSize.z };
    }
    else {
        return; // ��ȿ�� ���� �ƴ� ��� �Լ� ����
    }

    // ������ 2D�� ����
    for (int i = 0; i < 4; ++i) {
        if (!Project3DTo2D(cam, vertices[i], points[i])) {
            allPointsProjected = false;
        }
    }

    // ��� ���� �������� �ʾҴٸ� �׸��� ����
    if (!allPointsProjected) {
        return;
    }

    // �޸� DC ���� �� ���� �̹����� �޸� DC�� ����
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, frameWidth, frameHeight);
    HBITMAP oldMemBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
    image.Draw(memDC, 0, 0);  // �̹����� �޸� DC�� �׸�

    // ���� ������ ���� ����ũ ��Ʈ�� ����
    HBITMAP maskBitmap = CreateBitmap(frameWidth, frameHeight, 1, 1, NULL);
    HDC maskDC = CreateCompatibleDC(hdc);
    HBITMAP oldMaskBitmap = (HBITMAP)SelectObject(maskDC, maskBitmap);

    // ������� ����ũ�� ���� (������ ����� �����ϰ�)
    for (int y = 0; y < frameHeight; ++y) {
        for (int x = 0; x < frameWidth; ++x) {
            COLORREF color = GetPixel(memDC, x, y);
            SetPixel(maskDC, x, y, (color == RGB(0, 0, 0)) ? RGB(0, 0, 0) : RGB(255, 255, 255));
        }
    }

    // ������ �̹����� �׸���
    PlgBlt(hdc, points, memDC, 0, 0, frameWidth, frameHeight, maskBitmap, 0, 0);

    // Ŭ����
    SelectObject(memDC, oldMemBitmap);
    SelectObject(maskDC, oldMaskBitmap);
    DeleteObject(maskBitmap);
    DeleteDC(maskDC);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
}
