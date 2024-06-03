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
    // GDI+ �ʱ�ȭ
    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    int frameWidth = image.GetWidth();
    int frameHeight = image.GetHeight();

    if (frameWidth <= 0 || frameHeight <= 0) {
        GdiplusShutdown(gdiplusToken);
        return; // ��ȿ���� ���� ������ ũ��
    }

    Vector3 center = getPosition();
    Vector3 size = getSize();
    Vector3 halfSize = size / 2.0f;

    Vector3 vertices[4];
    POINT points[4];
    bool allPointsProjected = true;

    // ȸ�� �� ���� �ɼ�
    bool rotate90 = false;        // 90�� ȸ��
    bool flipHorizontal = false;  // ���� ����
    bool flipVertical = true;     // ���� ����

    if (size.y == 0) {
        vertices[0] = { center.x - halfSize.x, center.y, center.z - halfSize.z };
        vertices[1] = { center.x + halfSize.x, center.y, center.z - halfSize.z };
        vertices[2] = { center.x - halfSize.x, center.y, center.z + halfSize.z };
        vertices[3] = { center.x + halfSize.x, center.y, center.z + halfSize.z };
    }
    else if (size.z == 0) {
        vertices[0] = { center.x - halfSize.x, center.y - halfSize.y, center.z };
        vertices[1] = { center.x + halfSize.x, center.y - halfSize.y, center.z };
        vertices[2] = { center.x - halfSize.x, center.y + halfSize.y, center.z };
        vertices[3] = { center.x + halfSize.x, center.y + halfSize.y, center.z };
    }
    else if (size.x == 0) {
        vertices[0] = { center.x, center.y - halfSize.y, center.z - halfSize.z };
        vertices[1] = { center.x, center.y + halfSize.y, center.z - halfSize.z };
        vertices[2] = { center.x, center.y - halfSize.y, center.z + halfSize.z };
        vertices[3] = { center.x, center.y + halfSize.y, center.z + halfSize.z };
    }
    else {
        GdiplusShutdown(gdiplusToken);
        return; // ��ȿ�� ���� �ƴ� ��� �Լ� ����
    }

    for (int i = 0; i < 4; ++i) {
        if (!Project3DTo2D(cam, vertices[i], points[i])) {
            allPointsProjected = false;
        }
    }

    // points[2] ���۹�
    points[2].x += (points[3].x - points[1].x);

    if (!allPointsProjected) {
        GdiplusShutdown(gdiplusToken);
        return;
    }
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, frameWidth, frameHeight);
    HBITMAP oldMemBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

    image.Draw(memDC, 0, 0);  // �̹����� �޸� DC�� �׸�

    // GDI+ ��Ʈ������ ��ȯ
    Bitmap bitmap(memBitmap, NULL);
    Bitmap* pTransformedBitmap = bitmap.Clone(0, 0, frameWidth, frameHeight, PixelFormat32bppARGB);

    // �̹��� ȸ�� �� ����
    if (rotate90) {
        pTransformedBitmap->RotateFlip(Rotate90FlipNone);
    }
    else if (flipHorizontal && flipVertical) {
        pTransformedBitmap->RotateFlip(RotateNoneFlipXY);
    }
    else if (flipHorizontal) {
        pTransformedBitmap->RotateFlip(RotateNoneFlipX);
    }
    else if (flipVertical) {
        pTransformedBitmap->RotateFlip(RotateNoneFlipY);
    }

    // ������ �̹����� �������� ����ũ ����
    HDC transformedDC = CreateCompatibleDC(hdc);
    HBITMAP transformedBitmap;
    pTransformedBitmap->GetHBITMAP(Color(0, 0, 0), &transformedBitmap);
    SelectObject(transformedDC, transformedBitmap);

    HBITMAP maskBitmap = CreateBitmap(frameWidth, frameHeight, 1, 1, NULL);
    HDC maskDC = CreateCompatibleDC(hdc);
    HBITMAP oldMaskBitmap = (HBITMAP)SelectObject(maskDC, maskBitmap);

    // �̹����� ���� ���� �ĺ� �� ����ũ ����
    COLORREF transparentColor = RGB(0, 0, 0); // ���� ���� ���� (���⼭�� ������)
    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = frameWidth;
    bmi.bmiHeader.biHeight = -frameHeight; // top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // 32-bit color depth
    bmi.bmiHeader.biCompression = BI_RGB;
    BYTE* imageData = new BYTE[frameWidth * frameHeight * 4];
    GetDIBits(transformedDC, transformedBitmap, 0, frameHeight, imageData, &bmi, DIB_RGB_COLORS);
    for (int y = 0; y < frameHeight; ++y) {
        for (int x = 0; x < frameWidth; ++x) {
            COLORREF color = RGB(
                imageData[(y * frameWidth + x) * 4 + 2], // R
                imageData[(y * frameWidth + x) * 4 + 1], // G
                imageData[(y * frameWidth + x) * 4]      // B
            );
            if (color == transparentColor) {
                SetPixel(maskDC, x, y, transparentColor);
            }
            else {
                SetPixel(maskDC, x, y, RGB(255, 255, 255)); // �������� ������� ����
            }
        }
    }
    delete[] imageData;

    // ������ �̹����� �׸���
    PlgBlt(hdc, points, transformedDC, 0, 0, frameWidth, frameHeight, maskBitmap, 0, 0);

    // Ŭ����
    delete pTransformedBitmap;
    SelectObject(transformedDC, memBitmap);
    SelectObject(maskDC, oldMaskBitmap);
    DeleteObject(maskBitmap);
    DeleteDC(maskDC);
    DeleteObject(transformedBitmap);
    DeleteDC(transformedDC);

    // GDI+ ����
    GdiplusShutdown(gdiplusToken);
}
