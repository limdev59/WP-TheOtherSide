#pragma once
#include <Windows.h>

class Mouse {
public:
    Mouse();
    void OnMouseDown(WPARAM button, LPARAM lParam);
    void OnMouseUp(WPARAM button, LPARAM lParam);

    bool IsLeftClick() const;
    bool IsRightClick() const;

    POINT GetStartPosL() const;
    POINT GetEndPosL() const;
    POINT GetStartPosR() const;
    POINT GetEndPosR() const;

private:
    bool isLeftClick;
    bool isRightClick;
    POINT startPosL;
    POINT endPosL;
    POINT startPosR;
    POINT endPosR;
};
