#pragma once

#include "Vector3.h"
#include "Camera.h"
#include <atlimage.h>

class Object3D {
public:
    Object3D();
    Object3D(Vector3 position, Vector3 size );

    Vector3 getPosition() const;
    Vector3 getSize() const;

    void setPosition(const Vector3& newPosition);
    void setSize(const Vector3& newSize);

    virtual void DrawObject3D(HDC hdc, const Camera& cam);

private:
    Vector3 position3D;
    Vector3 size;
};


