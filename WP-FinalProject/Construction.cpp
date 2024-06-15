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
    POINT projectedPoints[4]; // ������ ���� ������ ���� �̸� �Ҵ�

    // ��ü�� ũ�⸦ ������� �� ���� ����
    if (size.y == 0) {
        // �ٴ�
        vertices[0] = { center.x - halfSize.x, center.y, center.z - halfSize.z };
        vertices[1] = { center.x + halfSize.x, center.y, center.z - halfSize.z };
        vertices[2] = { center.x + halfSize.x, center.y, center.z + halfSize.z };
        vertices[3] = { center.x - halfSize.x, center.y, center.z + halfSize.z };
    }
    else if (size.z == 0) {
        // ���κ�
        vertices[0] = { center.x - halfSize.x, center.y - halfSize.y, center.z };
        vertices[1] = { center.x + halfSize.x, center.y - halfSize.y, center.z };
        vertices[2] = { center.x + halfSize.x, center.y + halfSize.y, center.z };
        vertices[3] = { center.x - halfSize.x, center.y + halfSize.y, center.z };
    }
    else if (size.x == 0) {
        // ���κ�
        vertices[0] = { center.x, center.y - halfSize.y, center.z - halfSize.z };
        vertices[1] = { center.x, center.y + halfSize.y, center.z - halfSize.z };
        vertices[2] = { center.x, center.y + halfSize.y, center.z + halfSize.z };
        vertices[3] = { center.x, center.y - halfSize.y, center.z + halfSize.z };
    }
    else {
        return; // ��ȿ�� ���� �ƴ� ��� �Լ� ����
    }

    int numProjectedPoints = 0; // ������ ���� �� �ʱ�ȭ

    // ������ 2D�� ����
    for (int i = 0; i < 4; ++i) {
        if (Project3DTo2D(cam, vertices[i], points[i])) {
            // ������ ���� ����
            projectedPoints[numProjectedPoints++] = points[i];
        }
    }

    // ��� 3���� ���� �����Ǿ��ٸ� �׸���
    if (numProjectedPoints >= 3) {
        HBRUSH br = CreateSolidBrush(getBrushColor());
        HPEN pen = CreatePen(0, 2, getPenColor());
        SelectObject(hdc, br);
        SelectObject(hdc, pen);

        // ������ ���� ����Ͽ� Polygon �׸���
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
//    bool isProjected[4] = { false }; // ������ �� ���θ� ������ �迭
//
//    // ��ü�� ũ�⸦ ������� �� ���� ����
//    if (size.y == 0) {
//        // �ٴ�
//        vertices[0] = { center.x - halfSize.x, center.y, center.z - halfSize.z };
//        vertices[1] = { center.x + halfSize.x, center.y, center.z - halfSize.z };
//        vertices[2] = { center.x + halfSize.x, center.y, center.z + halfSize.z };
//        vertices[3] = { center.x - halfSize.x, center.y, center.z + halfSize.z };
//    }
//    else if (size.z == 0) {
//        // ���κ�
//        vertices[0] = { center.x - halfSize.x, center.y - halfSize.y, center.z };
//        vertices[1] = { center.x + halfSize.x, center.y - halfSize.y, center.z };
//        vertices[2] = { center.x + halfSize.x, center.y + halfSize.y, center.z };
//        vertices[3] = { center.x - halfSize.x, center.y + halfSize.y, center.z };
//    }
//    else if (size.x == 0) {
//        // ���κ�
//        vertices[0] = { center.x, center.y - halfSize.y, center.z - halfSize.z };
//        vertices[1] = { center.x, center.y + halfSize.y, center.z - halfSize.z };
//        vertices[2] = { center.x, center.y + halfSize.y, center.z + halfSize.z };
//        vertices[3] = { center.x, center.y - halfSize.y, center.z + halfSize.z };
//    }
//    else {
//        return; // ��ȿ�� ���� �ƴ� ��� �Լ� ����
//    }
//
//    int numProjectedPoints = 0; // ������ ���� �� �ʱ�ȭ
//
//    // ������ 2D�� ����
//    for (int i = 0; i < 4; ++i) {
//        if (Project3DTo2D(cam, vertices[i], points[i])) {
//            isProjected[i] = true;
//            numProjectedPoints++;
//        }
//    }
//
//    // �ּ� �� ���� ���� �����Ǿ��ٸ� �׸���
//    if (numProjectedPoints >= 2) {
//        POINT projectedPoints[4];
//        int projectedIndex = 0;
//
//        // ������ ���� ����Ͽ� �迭�� ����
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
//        // Ŭ���ε� ������ �ٰ��� �׸���
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
