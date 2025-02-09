// Mouse.cpp
#include "Mouse.h"
#include "Vector3.h"
#include "Camera.h"

Mouse::Mouse() : isLeftClick(false), isRightClick(false) {
	ZeroMemory(&startPosL, sizeof(POINT));
	ZeroMemory(&endPosL, sizeof(POINT));
	ZeroMemory(&startPosR, sizeof(POINT));
	ZeroMemory(&endPosR, sizeof(POINT));
	ZeroMemory(&mousePos, sizeof(POINT));
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

void Mouse::OnMouseLUp(WPARAM button, LPARAM lParam) {
	isLeftClick = false;
	endPosL.x = LOWORD(lParam);
	endPosL.y = HIWORD(lParam);

}
void Mouse::OnMouseRUp(WPARAM button, LPARAM lParam) {
	isRightClick = false;
	endPosR.x = LOWORD(lParam);
	endPosR.y = HIWORD(lParam);
}

void Mouse::OnMouseMove(LPARAM lParam) {
	mousePos.x = LOWORD(lParam);
	mousePos.y = HIWORD(lParam);
}

bool Mouse::IsLeftClick() const {
	return isLeftClick;
}

bool Mouse::IsRightClick() const {
	return isRightClick;
}

POINT Mouse::getStartPosL() const {
	return startPosL;
}

POINT Mouse::getEndPosL() const {
	return endPosL;
}

POINT Mouse::getStartPosR() const {
	return startPosR;
}

POINT Mouse::getEndPosR() const {
	return endPosR;
}

POINT Mouse::getMousePosition() const {
	return mousePos;
}

Vector3 Mouse::getMouse3DPosition() const {
	return mouse3DPos;
}

void Mouse::UpdateMouse3DPosition(const Camera& cam) {
	mouse3DPos = Unproject2DTo3D(cam, mousePos, 0.0f);
}
