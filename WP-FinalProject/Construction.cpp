#include "Vector3.h"
#include "Construction.h"
#include "Camera.h"
#include <windows.h>
#include <vector>

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
    POINT projectedPoints[4]; // 투영된 점을 저장할 공간 미리 할당

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

    int numProjectedPoints = 0; // 투영된 점의 수 초기화

    // 점들을 2D로 투영
    for (int i = 0; i < 4; ++i) {
        if (Project3DTo2D(cam, vertices[i], points[i])) {
            // 투영된 점을 저장
            projectedPoints[numProjectedPoints++] = points[i];
        }
    }

    // 적어도 3개의 점이 투영되었다면 그리기
    if (numProjectedPoints >= 3) {
        HBRUSH br = CreateSolidBrush(getBrushColor());
        HPEN pen = CreatePen(0, 2, getPenColor());
        SelectObject(hdc, br);
        SelectObject(hdc, pen);

        // 투영된 점만 사용하여 Polygon 그리기
        Polygon(hdc, projectedPoints, numProjectedPoints);
        DeleteObject(br);
        DeleteObject(pen);
    }
}

bool compareByZ(const Construction& a, const Construction& b) {
    return a.getPosition().z > b.getPosition().z;
}

//void ClipAndDrawPolygon(HDC hdc, POINT* points, int count);
//void Construction::DrawObject3D(HDC hdc, const Camera& cam) {
//    Vector3 center = getPosition();
//    Vector3 size = getSize();
//    Vector3 halfSize = size / 2.0f;
//
//    Vector3 vertices[4];
//    POINT points[4];
//    bool isProjected[4] = { false }; // 투영된 점 여부를 저장할 배열
//
//    // 객체의 크기를 기반으로 면 유형 결정
//    if (size.y == 0) {
//        // 바닥
//        vertices[0] = { center.x - halfSize.x, center.y, center.z - halfSize.z };
//        vertices[1] = { center.x + halfSize.x, center.y, center.z - halfSize.z };
//        vertices[2] = { center.x + halfSize.x, center.y, center.z + halfSize.z };
//        vertices[3] = { center.x - halfSize.x, center.y, center.z + halfSize.z };
//    }
//    else if (size.z == 0) {
//        // 가로벽
//        vertices[0] = { center.x - halfSize.x, center.y - halfSize.y, center.z };
//        vertices[1] = { center.x + halfSize.x, center.y - halfSize.y, center.z };
//        vertices[2] = { center.x + halfSize.x, center.y + halfSize.y, center.z };
//        vertices[3] = { center.x - halfSize.x, center.y + halfSize.y, center.z };
//    }
//    else if (size.x == 0) {
//        // 세로벽
//        vertices[0] = { center.x, center.y - halfSize.y, center.z - halfSize.z };
//        vertices[1] = { center.x, center.y + halfSize.y, center.z - halfSize.z };
//        vertices[2] = { center.x, center.y + halfSize.y, center.z + halfSize.z };
//        vertices[3] = { center.x, center.y - halfSize.y, center.z + halfSize.z };
//    }
//    else {
//        return; // 유효한 면이 아닌 경우 함수 종료
//    }
//
//    int numProjectedPoints = 0; // 투영된 점의 수 초기화
//
//    // 점들을 2D로 투영
//    for (int i = 0; i < 4; ++i) {
//        if (Project3DTo2D(cam, vertices[i], points[i])) {
//            isProjected[i] = true;
//            numProjectedPoints++;
//        }
//    }
//
//    // 최소 두 개의 점이 투영되었다면 그리기
//    if (numProjectedPoints >= 2) {
//        POINT projectedPoints[4];
//        int projectedIndex = 0;
//
//        // 투영된 점만 사용하여 배열에 저장
//        for (int i = 0; i < 4; ++i) {
//            if (isProjected[i]) {
//                projectedPoints[projectedIndex++] = points[i];
//            }
//        }
//
//        HBRUSH br = CreateSolidBrush(getBrushColor());
//        HPEN pen = CreatePen(0, 2, getPenColor());
//        SelectObject(hdc, br);
//        SelectObject(hdc, pen);
//
//        // 클리핑된 점으로 다각형 그리기
//        ClipAndDrawPolygon(hdc, projectedPoints, projectedIndex);
//
//        DeleteObject(br);
//        DeleteObject(pen);
//    }
//}
//
//void ClipAndDrawPolygon(HDC hdc, POINT* points, int count) {
//    RECT rect{0,0,1600,900};
//    int minX = rect.left;
//    int maxX = rect.right;
//    int minY = rect.top;
//    int maxY = rect.bottom;
//
//    for (int i = 0; i < count; ++i) {
//        points[i].x = max(minX, min(maxX, points[i].x));
//        points[i].y = max(minY, min(maxY, points[i].y));
//    }
//
//    Polygon(hdc, points, count);
//}
