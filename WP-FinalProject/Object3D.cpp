#include "Object3D.h"
#include "Camera.h"
#include <Windows.h>
#include <vector>

Object3D::Object3D() : position{ 0, 0, 0 }, size{ 1, 1, 1 } {}
Object3D::Object3D(Vector3 position, Vector3 size)
    : position(position), size(size) {}
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
void Object3D::DrawObject3D(HDC hdc, const Camera& cam) {
    
}
