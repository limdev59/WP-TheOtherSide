// Mouse.h
#pragma once
#include <Windows.h>
#include "Camera.h"
#include "Vector3.h"

class Mouse {
public:
    Mouse();
    void OnMouseDown(WPARAM button, LPARAM lParam);
    void OnMouseLUp(WPARAM button, LPARAM lParam);
    void OnMouseRUp(WPARAM button, LPARAM lParam);
    void OnMouseMove(LPARAM lParam);

    bool IsLeftClick() const;
    bool IsRightClick() const;

    POINT getStartPosL() const;
    POINT getEndPosL() const;
    POINT getStartPosR() const;
    POINT getEndPosR() const;
    POINT getMousePosition() const;
    Vector3 getMouse3DPosition() const;
    void UpdateMouse3DPosition(const Camera& cam);

private:
    bool isLeftClick;
    bool isRightClick;
    POINT startPosL;
    POINT endPosL;
    POINT startPosR;
    POINT endPosR;
    POINT mousePos;

    Vector3 mouse3DPos;
};
