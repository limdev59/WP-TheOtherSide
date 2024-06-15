// Actor.cpp
#include "Vector3.h"
#include "Actor.h"
#include <gdiplus.h>
#include <iostream>
#include <string>
using namespace Gdiplus;

Actor::Actor(Vector3 position, Vector3 size)
    : Object3D(position, size), hitbox({ 0, 0, 0, 0 }), animationController("default") {
    // 초기화 코드
}

Actor::~Actor() {
    // 소멸자 코드 (필요한 경우)
}

void Actor::set2DPosition(float x, float z) {
    setPosition({x,getPosition().y, z});
}

POINT Actor::get2DPosition() const {
    return POINT{(long)getPosition().x, (long)getPosition().z };
}

void Actor::move2DPosition(float dx, float dy) {
    movePosition(dx, 0, dy);
    // 히트박스도 함께 이동시킴
    hitbox.left += dx;
    hitbox.right += dx;
    hitbox.top += dy;
    hitbox.bottom += dy;
}

void Actor::setHitbox(int width, int height) {
    hitbox.left = getPosition().x - width / 2;
    hitbox.top = getPosition().z - height / 2;
    hitbox.right = getPosition().x + width / 2;
    hitbox.bottom = getPosition().z + height / 2;
}

RECT Actor::getHitbox() const {
    return hitbox;
}

void Actor::setAnimationController(const AnimationController& animationController) {
    this->animationController = animationController;
}

AnimationController& Actor::getAnimationController() {
    return animationController;
}

void Actor::DrawObject3D(HDC hdc, const Camera& cam) {
    // GDI+ 초기화
    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    const CImage* currentFrame = animationController.getCurrentFrame();
    if (!currentFrame) {
        GdiplusShutdown(gdiplusToken);
        return;
    }
    int frameWidth = currentFrame->GetWidth();
    int frameHeight = currentFrame->GetHeight();

    if (frameWidth <= 0 || frameHeight <= 0) {
        GdiplusShutdown(gdiplusToken);
        return;
    }

    Vector3 center = getPosition();
    Vector3 size = getSize();
    Vector3 halfSize = size / 2.0f;

    Vector3 vertices[4];
    POINT points[4];
    bool allPointsProjected = true;

    // 회전 및 반전 옵션
    bool rotate90 = false;        // 90도 회전
    bool flipHorizontal = false;  // 수평 반전
    bool flipVertical = true;     // 수직 반전

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
        return;
    }

    for (int i = 0; i < 4; ++i) {
        if (!Project3DTo2D(cam, vertices[i], points[i])) {
            allPointsProjected = false;
        }
    }
    points[0].x = points[0].x + (points[3].x - points[1].x)/2;
    points[2].x = points[2].x + (points[3].x - points[1].x)/2;
    if (!allPointsProjected) {
        GdiplusShutdown(gdiplusToken);
        return;
    }

    Bitmap* pTransformedBitmap = CreateTransformedBitmap(hdc, currentFrame, rotate90, flipHorizontal, flipVertical);
    if (pTransformedBitmap == nullptr) {
        GdiplusShutdown(gdiplusToken);
        return;
    }

    HBITMAP maskBitmap = CreateMask(hdc, pTransformedBitmap, frameWidth, frameHeight, cam);

    HDC transformedDC = CreateCompatibleDC(hdc);
    HBITMAP transformedBitmap;
    pTransformedBitmap->GetHBITMAP(Color(0, 0, 0), &transformedBitmap);
    SelectObject(transformedDC, transformedBitmap);

    PlgBlt(hdc, points, transformedDC, 0, 0, frameWidth, frameHeight, maskBitmap, 0, 0);

    {
        Graphics graphics(hdc);
        Font font(L"Arial", 16);
        SolidBrush brush(Color(255, 255, 255));

        std::wstring yawStr = L"Yaw: " + std::to_wstring(cam.getYaw());
        std::wstring pitchStr = L"Pitch: " + std::to_wstring(cam.getPitch());
        std::wstring rollStr = L"Roll: " + std::to_wstring(cam.getRoll());

        graphics.DrawString(yawStr.c_str(), -1, &font, PointF(10, 10), &brush);
        graphics.DrawString(pitchStr.c_str(), -1, &font, PointF(10, 30), &brush);
        graphics.DrawString(rollStr.c_str(), -1, &font, PointF(10, 50), &brush);
    }

    delete pTransformedBitmap;
    DeleteObject(maskBitmap);
    DeleteObject(transformedBitmap);
    DeleteDC(transformedDC);

    GdiplusShutdown(gdiplusToken);
}


Bitmap* Actor::CreateTransformedBitmap(HDC hdc, const CImage* image, bool rotate90, bool flipHorizontal, bool flipVertical) {
    int frameWidth = image->GetWidth();
    int frameHeight = image->GetHeight();

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, frameWidth, frameHeight);
    HBITMAP oldMemBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

    image->Draw(memDC, 0, 0);  // 이미지를 메모리 DC에 그림

    // GDI+ 비트맵으로 변환
    Bitmap bitmap(memBitmap, NULL);
    Bitmap* pTransformedBitmap = bitmap.Clone(0, 0, frameWidth, frameHeight, PixelFormat32bppARGB);

    // 이미지 회전 및 반전
    if (rotate90) {
        pTransformedBitmap->RotateFlip(Rotate90FlipNone);
    }
    if (flipHorizontal && flipVertical) {
        pTransformedBitmap->RotateFlip(RotateNoneFlipXY);
    }
    else if (flipHorizontal) {
        pTransformedBitmap->RotateFlip(RotateNoneFlipX);
    }
    else if (flipVertical) {
        pTransformedBitmap->RotateFlip(RotateNoneFlipY);
    }

    SelectObject(memDC, oldMemBitmap);
    DeleteDC(memDC);
    DeleteObject(memBitmap);

    return pTransformedBitmap;
}

HBITMAP Actor::CreateMask(HDC hdc, Bitmap* pTransformedBitmap, int frameWidth, int frameHeight, Camera cam) {
    HDC transformedDC = CreateCompatibleDC(hdc);
    HBITMAP transformedBitmap;
    pTransformedBitmap->GetHBITMAP(Color(0, 0, 0), &transformedBitmap);
    SelectObject(transformedDC, transformedBitmap);

    HBITMAP maskBitmap = CreateBitmap(frameWidth, frameHeight, 1, 1, NULL);
    HDC maskDC = CreateCompatibleDC(hdc);
    HBITMAP oldMaskBitmap = (HBITMAP)SelectObject(maskDC, maskBitmap);

    COLORREF transparentColor = RGB(0, 0, 0);
    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = frameWidth;
    bmi.bmiHeader.biHeight = (cam.getYaw()==0.02&&-0.44<=cam.getPitch()&& 0.04<= cam.getPitch())? -frameHeight :frameHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    BYTE* imageData = new BYTE[frameWidth * frameHeight * 4];
    GetDIBits(transformedDC, transformedBitmap, 0, frameHeight, imageData, &bmi, DIB_RGB_COLORS);

    // Invert the height for mask creation to account for coordinate system differences
    for (int y = 0; y < frameHeight; ++y) {
        for (int x = 0; x < frameWidth; ++x) {
            COLORREF color = RGB(
                imageData[(y * frameWidth + x) * 4 + 2],
                imageData[(y * frameWidth + x) * 4 + 1],
                imageData[(y * frameWidth + x) * 4]
            );
            if (color == transparentColor) {
                SetPixel(maskDC, x, frameHeight - 1 - y, transparentColor);
            }
            else {
                SetPixel(maskDC, x, frameHeight - 1 - y, RGB(255, 255, 255));
            }
        }
    }
    delete[] imageData;

    SelectObject(maskDC, oldMaskBitmap);
    DeleteDC(maskDC);
    DeleteDC(transformedDC);
    DeleteObject(transformedBitmap);

    return maskBitmap;
}

