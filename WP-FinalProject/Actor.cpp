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
    // GDI+ 초기화
    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    int frameWidth = image.GetWidth();
    int frameHeight = image.GetHeight();

    if (frameWidth <= 0 || frameHeight <= 0) {
        GdiplusShutdown(gdiplusToken);
        return; // 유효하지 않은 프레임 크기
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
        return; // 유효한 면이 아닌 경우 함수 종료
    }

    for (int i = 0; i < 4; ++i) {
        if (!Project3DTo2D(cam, vertices[i], points[i])) {
            allPointsProjected = false;
        }
    }

    // points[2] 조작문
    points[2].x += (points[3].x - points[1].x);

    if (!allPointsProjected) {
        GdiplusShutdown(gdiplusToken);
        return;
    }
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, frameWidth, frameHeight);
    HBITMAP oldMemBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

    image.Draw(memDC, 0, 0);  // 이미지를 메모리 DC에 그림

    // GDI+ 비트맵으로 변환
    Bitmap bitmap(memBitmap, NULL);
    Bitmap* pTransformedBitmap = bitmap.Clone(0, 0, frameWidth, frameHeight, PixelFormat32bppARGB);

    // 이미지 회전 및 반전
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

    // 변형된 이미지를 기준으로 마스크 생성
    HDC transformedDC = CreateCompatibleDC(hdc);
    HBITMAP transformedBitmap;
    pTransformedBitmap->GetHBITMAP(Color(0, 0, 0), &transformedBitmap);
    SelectObject(transformedDC, transformedBitmap);

    HBITMAP maskBitmap = CreateBitmap(frameWidth, frameHeight, 1, 1, NULL);
    HDC maskDC = CreateCompatibleDC(hdc);
    HBITMAP oldMaskBitmap = (HBITMAP)SelectObject(maskDC, maskBitmap);

    // 이미지의 투명 색상 식별 및 마스크 설정
    COLORREF transparentColor = RGB(0, 0, 0); // 투명 색상 설정 (여기서는 검은색)
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
                SetPixel(maskDC, x, y, RGB(255, 255, 255)); // 나머지는 흰색으로 설정
            }
        }
    }
    delete[] imageData;

    // 변형된 이미지를 그리기
    PlgBlt(hdc, points, transformedDC, 0, 0, frameWidth, frameHeight, maskBitmap, 0, 0);

    // 클린업
    delete pTransformedBitmap;
    SelectObject(transformedDC, memBitmap);
    SelectObject(maskDC, oldMaskBitmap);
    DeleteObject(maskBitmap);
    DeleteDC(maskDC);
    DeleteObject(transformedBitmap);
    DeleteDC(transformedDC);

    // GDI+ 종료
    GdiplusShutdown(gdiplusToken);
}
