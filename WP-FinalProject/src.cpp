#include <windows.h>
#include <atlimage.h>
#include "Camera.h"
#include "Object3D.h"

// 전역 상수
constexpr int gameTick = 16;

// 전역 변수
bool keyStates[256] = { 0 };
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program";

static HDC hDC, mDC;
static PAINTSTRUCT ps;
static HBITMAP hBitmap;
static RECT rt;

// 함수 선언
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void HandleCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HandlePaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HandleResize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HandleMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HandleLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HandleLButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HandleKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HandleKeyUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HandleTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 초기화된 카메라와 객체
static Camera camera({ 0, 2.6f, 3 }, 0, 0);
static Object3D floor1({ 0, 0, 10 }, { 10, 0, 10 });
static Object3D floor2({ 10, 0, 10 }, { 10, 0, 10 });
static Object3D wall({ 0, 5, 15 }, { 10, 10, 0 });
static Object3D wall2({ 0, 5, 5 }, { 10, 10, 0 });
static CImage image;

// WinMain 함수
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
    HWND hWnd;
    MSG Message;
    WNDCLASS WndClass;
    g_hInst = hInstance;

    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;

    RegisterClass(&WndClass);

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, -10, 0,
        1600, 900, NULL, (HMENU)NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);

    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return (int)Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        HandleCreate(hWnd, uMsg, wParam, lParam);
        return 0;
    case WM_PAINT:
        HandlePaint(hWnd, uMsg, wParam, lParam);
        return 0;
    case WM_SIZE:
        HandleResize(hWnd, uMsg, wParam, lParam);
        return 0;
    case WM_MOUSEMOVE:
        HandleMouseMove(hWnd, uMsg, wParam, lParam);
        return 0;
    case WM_LBUTTONDOWN:
        HandleLButtonDown(hWnd, uMsg, wParam, lParam);
        return 0;
    case WM_LBUTTONUP:
        HandleLButtonUp(hWnd, uMsg, wParam, lParam);
        return 0;
    case WM_KEYDOWN:
        HandleKeyDown(hWnd, uMsg, wParam, lParam);
        return 0;
    case WM_KEYUP:
        HandleKeyUp(hWnd, uMsg, wParam, lParam);
        return 0;
    case WM_TIMER:
        HandleTimer(hWnd, uMsg, wParam, lParam);
        return 0;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        return 0;
    }
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}
void HandleCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    GetClientRect(hWnd, &rt);
    if (FAILED(image.Load(TEXT("Horror_background.jpg")))) {
        MessageBox(hWnd, TEXT("Failed to load image"), TEXT("Error"), MB_OK);
        PostQuitMessage(0);
    }
    SetTimer(hWnd, 1, gameTick, NULL);
}
void HandlePaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    hDC = BeginPaint(hWnd, &ps);
    mDC = CreateCompatibleDC(hDC);
    hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
    SelectObject(mDC, hBitmap);
    HBRUSH br = CreateSolidBrush(RGB(25, 0, 55));
    FillRect(mDC, &rt, br);
    DeleteObject(br);
    image.Draw(mDC, 0, 0, rt.right, rt.bottom);
    
    DrawObject3D(mDC, camera, floor1);
    DrawObject3D(mDC, camera, floor2);
    DrawObject3D(mDC, camera, wall);
    DrawObject3D(mDC, camera, wall2);

    BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
    DeleteDC(mDC);
    DeleteObject(hBitmap);
    EndPaint(hWnd, &ps);
}
void HandleResize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    GetClientRect(hWnd, &rt);
    DeleteObject(hBitmap);
    hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
    SelectObject(mDC, hBitmap);
}
void HandleMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // 필요 시 구현
}
void HandleLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // 필요 시 구현
}
void HandleLButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // 필요 시 구현
}
void HandleKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    keyStates[wParam] = true;
}
void HandleKeyUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    keyStates[wParam] = false;
}
void HandleTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (keyStates['A']) {
        camera.move({ -0.1f, 0, 0 });
    }
    if (keyStates['D']) {
        camera.move({ 0.1f, 0, 0 });
    }
    if (keyStates['W']) {
        camera.move({ 0, 0, 0.1f });
    }
    if (keyStates['S']) {
        camera.move({ 0, 0, -0.1f });
    }
    if (keyStates[VK_SPACE]) {
        camera.move({ 0, 0.1f, 0 });
    }
    if (keyStates[VK_SHIFT]) {
        camera.move({ 0, -0.1f, 0 });
    }
    if (keyStates[VK_LEFT]) {
        camera.rotate(-0.02f, 0);
    }
    if (keyStates[VK_RIGHT]) {
        camera.rotate(0.02f, 0);
    }
    if (keyStates[VK_UP]) {
        camera.rotate(0, 0.02f);
    }
    if (keyStates[VK_DOWN]) {
        camera.rotate(0, -0.02f);
    }

    InvalidateRect(hWnd, NULL, FALSE);
}
