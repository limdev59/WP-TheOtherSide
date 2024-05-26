#include "Vector3.h"
#include "Construction.h"
#include "Camera.h"
#include <windows.h>


Construction::Construction() : penColor(RGB(0, 0, 0)), brushColor(RGB(255, 255, 255)) {}
Construction::Construction(Vector3 position, Vector3 size)
    : Object3D(position, size), penColor(RGB(0, 0, 0)), brushColor(RGB(255, 255, 255)) {}
Construction::Construction(Vector3 position, Vector3 size, COLORREF brushColor, COLORREF penColor)
    : Object3D(position, size), penColor(penColor), brushColor(brushColor) {}

COLORREF Construction::getPenColor() const {
    return penColor;
}
COLORREF Construction::getBrushColor() const {
    return brushColor;
}

void Construction::setPenColor(COLORREF color) {
    penColor = color;
}
void Construction::setBrushColor(COLORREF color) {
    brushColor = color;
}

void Construction::DrawObject3D(HDC hdc, const Camera& cam) {
    Vector3 center = getPosition();
    Vector3 size = getSize();
    Vector3 halfSize = size / 2.0f;

    Vector3 vertices[4];
    POINT points[4];
    bool allPointsProjected = true;

    // 객체의 크기를 기반으로 면 유형 결정
    if (size.y == 0) {
        // 바닥
        vertices[0] = { center.x - halfSize.x, center.y, center.z - halfSize.z };
        vertices[1] = { center.x + halfSize.x, center.y, center.z - halfSize.z };
        vertices[2] = { center.x + halfSize.x, center.y, center.z + halfSize.z };
        vertices[3] = { center.x - halfSize.x, center.y, center.z + halfSize.z };
    }
    else if (size.z == 0) {
        // 가로벽
        vertices[0] = { center.x - halfSize.x, center.y - halfSize.y, center.z };
        vertices[1] = { center.x + halfSize.x, center.y - halfSize.y, center.z };
        vertices[2] = { center.x + halfSize.x, center.y + halfSize.y, center.z };
        vertices[3] = { center.x - halfSize.x, center.y + halfSize.y, center.z };
    }
    else if (size.x == 0) {
        // 세로벽
        vertices[0] = { center.x, center.y - halfSize.y, center.z - halfSize.z };
        vertices[1] = { center.x, center.y + halfSize.y, center.z - halfSize.z };
        vertices[2] = { center.x, center.y + halfSize.y, center.z + halfSize.z };
        vertices[3] = { center.x, center.y - halfSize.y, center.z + halfSize.z };
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

    // 선택한 면 그리기
    HBRUSH br = CreateSolidBrush(getBrushColor());
    HPEN pen = CreatePen(0, 2, getPenColor());
    SelectObject(hdc, br);
    SelectObject(hdc, pen);

    Polygon(hdc, points, 4);
    DeleteObject(br);
    DeleteObject(pen);
}