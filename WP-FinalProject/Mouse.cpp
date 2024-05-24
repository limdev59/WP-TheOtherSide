#include "Mouse.h"

Mouse::Mouse() : isLeftClick(false), isRightClick(false) {
    ZeroMemory(&startPosL, sizeof(POINT));
    ZeroMemory(&endPosL, sizeof(POINT));
    ZeroMemory(&startPosR, sizeof(POINT));
    ZeroMemory(&endPosR, sizeof(POINT));
}

void Mouse::OnMouseDown(WPARAM button, LPARAM lParam) {
    if (button == MK_LBUTTON) {
        isLeftClick = true;
        startPosL.x = LOWORD(lParam);
        startPosL.y = HIWORD(lParam);
    }
    else if (button == MK_RBUTTON) {
        isRightClick = true;
        startPosR.x = LOWORD(lParam);
        startPosR.y = HIWORD(lParam);
    }
}

void Mouse::OnMouseUp(WPARAM button, LPARAM lParam) {
    if (button == MK_LBUTTON) {
        isLeftClick = false;
        endPosL.x = LOWORD(lParam);
        endPosL.y = HIWORD(lParam);
    }
    else if (button == MK_RBUTTON) {
        isRightClick = false;
        endPosR.x = LOWORD(lParam);
        endPosR.y = HIWORD(lParam);
    }
}

bool Mouse::IsLeftClick() const {
    return isLeftClick;
}

bool Mouse::IsRightClick() const {
    return isRightClick;
}

POINT Mouse::GetStartPosL() const {
    return startPosL;
}

POINT Mouse::GetEndPosL() const {
    return endPosL;
}

POINT Mouse::GetStartPosR() const {
    return startPosR;
}

POINT Mouse::GetEndPosR() const {
    return endPosR;
}
