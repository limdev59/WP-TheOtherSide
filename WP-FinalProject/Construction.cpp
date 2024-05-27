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

    // ������ �� �׸���
    HBRUSH br = CreateSolidBrush(getBrushColor());
    HPEN pen = CreatePen(0, 2, getPenColor());
    SelectObject(hdc, br);
    SelectObject(hdc, pen);

    Polygon(hdc, points, 4);
    DeleteObject(br);
    DeleteObject(pen);
}