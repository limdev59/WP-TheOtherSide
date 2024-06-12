#pragma once

#include "Vector3.h"
#include "Object3D.h"
#include "Camera.h"
#include <atlimage.h>

class Construction : public Object3D {
public:
    Construction();
    Construction(Vector3 position, Vector3 size);
    Construction(Vector3 position, Vector3 size, COLORREF brushColor, COLORREF penColor);

    // Override Object3D's functions
    COLORREF getPenColor() const;
    COLORREF getBrushColor() const;

    void setPenColor(COLORREF color);
    void setBrushColor(COLORREF color);

    // New function for Construction class
    void DrawObject3D(HDC hdc, const Camera& cam) override;
    bool ClipLineWithPlane(const Vector3& p0, const Vector3& p1, const Vector3& planePoint, const Vector3& planeNormal, Vector3& outPoint);

private:
    COLORREF penColor;    // 펜 색상
    COLORREF brushColor;  // 브러쉬 색상
};