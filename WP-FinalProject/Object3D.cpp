#include "Object3D.h"
#include "Camera.h"
#include <windows.h>
#include <vector>

Object3D::Object3D() : position{ 0, 0, 0 }, size{ 1, 1, 1 } {}

Object3D::Object3D(Vector3 position, Vector3 size) : position(position), size(size) {}

Vector3 Object3D::getPosition() const {
    return position;
}

Vector3 Object3D::getSize() const {
    return size;
}

void Object3D::setPosition(const Vector3& newPosition) {
    position = newPosition;
}

void Object3D::setSize(const Vector3& newSize) {
    size = newSize;
}

void DrawObject3D(HDC hdc, const Camera& cam, const Object3D& obj) {
    POINT points[8];
    Vector3 vertices[8] = {
        {obj.getPosition().x - obj.getSize().x / 2, obj.getPosition().y - obj.getSize().y / 2, obj.getPosition().z - obj.getSize().z / 2},
        {obj.getPosition().x + obj.getSize().x / 2, obj.getPosition().y - obj.getSize().y / 2, obj.getPosition().z - obj.getSize().z / 2},
        {obj.getPosition().x + obj.getSize().x / 2, obj.getPosition().y + obj.getSize().y / 2, obj.getPosition().z - obj.getSize().z / 2},
        {obj.getPosition().x - obj.getSize().x / 2, obj.getPosition().y + obj.getSize().y / 2, obj.getPosition().z - obj.getSize().z / 2},
        {obj.getPosition().x - obj.getSize().x / 2, obj.getPosition().y - obj.getSize().y / 2, obj.getPosition().z + obj.getSize().z / 2},
        {obj.getPosition().x + obj.getSize().x / 2, obj.getPosition().y - obj.getSize().y / 2, obj.getPosition().z + obj.getSize().z / 2},
        {obj.getPosition().x + obj.getSize().x / 2, obj.getPosition().y + obj.getSize().y / 2, obj.getPosition().z + obj.getSize().z / 2},
        {obj.getPosition().x - obj.getSize().x / 2, obj.getPosition().y + obj.getSize().y / 2, obj.getPosition().z + obj.getSize().z / 2},
    };

    bool allPointsProjected = true;
    for (int i = 0; i < 8; ++i) {
        if (!Project3DTo2D(cam, vertices[i], points[i])) {
            allPointsProjected = false;
        }
    }

    if (!allPointsProjected) {
        return;
    }

    POINT poly[4];
    
    SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
    SelectObject(hdc,(HPEN)(GetStockObject(BLACK_PEN)));
    

    poly[0] = points[4];
    poly[1] = points[5];
    poly[2] = points[6];
    poly[3] = points[7];
    Polygon(hdc, poly, 4);

    poly[0] = points[3];
    poly[1] = points[2];
    poly[2] = points[6];
    poly[3] = points[7];
    Polygon(hdc, poly, 4);
}
