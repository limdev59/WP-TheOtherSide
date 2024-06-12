#pragma once

#include <windows.h>
#include <atlimage.h>
#include <mmsystem.h>
#include "Camera.h"
#include "Object3D.h"
#include "Construction.h"
#include "Animation.h"
#include "Actor.h"
#include "AnimationController.h"
#include "Player.h"
#pragma comment(lib, "winmm.lib")

#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#define KEY_UP_CONDITION(KEY) (!keyStates[KEY] && KEY == wParam)

// 전역 상수
constexpr int gameTick = 7;
constexpr float cameraFollowSpeed = 0.1f; // 카메라가 플레이어를 따라오는 속도

// 전역 변수
bool keyStates[256] = { 0 };
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program";

static HDC hDC, mDC;
static PAINTSTRUCT ps;
static HBITMAP hBitmap;
static RECT rt;

// 애니메이션 컨트롤러와 시간 관련 변수
static AnimationController animationController("kitten_R_default");
static DWORD lastTime = timeGetTime();

// 함수 선언
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 초기화된 카메라와 객체
static Camera camera({ 0, 3.6f, 3 }, 0.02f, -0.4f, 0);
static Construction floor1({ 0, 0, 10 }, { 10, 0, 10 }, RGB(42, 32, 50), RGB(34, 15, 33));
static Construction floor2({ 10, 0, 10 }, { 10, 0, 10 }, RGB(42, 32, 50), RGB(34, 15, 33));

static Construction wall({ 0, 4, 15 }, { 10, 8, 0 }, RGB(24, 24, 40), RGB(24, 15, 33));
static Construction wall_({ 0, 1, 14 }, { 10, 2, 0 }, RGB(32, 32, 48), RGB(24, 15, 33));
static Construction wall2({ -5, 4, 10 }, { 0, 8, 10 }, RGB(20, 20, 36), RGB(24, 15, 33));
static Construction wall3({ 0, 1, 10 }, { 2, 2, 0 });

static Player player({ 0, 0.5, 10 }, { 2, 2, 0 });
static CImage image;

// 애니메이션 초기화 함수
void InitializeAnimations() {
    std::map<float, POINT> d_positions = {
        {0.0f, {0, 0}}
    };

    std::map<float, POINT> d_scales = {
        {0.0f, {1, 1}}
    };
    std::map<float, POINT> m_positions = {
        {0.0f, {0, 0}},
        {0.2f, {0, 0}},
        {0.4f, {0, 0}},
        {0.6f, {0, 0}},
        {0.8f, {0, 0}}
    };

    std::map<float, POINT> m_scales = {
        {0.0f, {1, 1}},
        {0.2f, {1, 1}},
        {0.4f, {1, 1}},
        {0.6f, {1, 1}},
        {0.8f, {1, 1}}
    };

    std::map<float, std::string> img_imagesKittenR = {
        {0.0f, "kitten_R_default_1"}
    };
    std::map<float, std::string> img_imagesKittenL = {
        {0.0f, "kitten_L_default_1"}
    };
    std::map<float, std::string> img_KittenMoveR = {
        {0.0f, "kitten_R_default_1"},
        {0.2f, "kitten_R_default_2"},
        {0.4f, "kitten_R_default_1"},
        {0.6f, "kitten_R_default_2"},
        {0.8f, "kitten_R_default_1"}
    };
    std::map<float, std::string> img_KittenMoveL = {
        {0.0f, "kitten_L_default_1"},
        {0.2f, "kitten_L_default_2"},
        {0.4f, "kitten_L_default_1"},
        {0.6f, "kitten_L_default_2"},
        {0.8f, "kitten_L_default_1"}
    };

    Animation Kitten_R_default("kitten_R_default", true, 0.0f, d_positions, d_scales, img_imagesKittenR);
    Animation Kitten_L_default("kitten_L_default", true, 0.0f, d_positions, d_scales, img_imagesKittenL);
    Animation Kitten_R_move   ("kitten_R_move",    true,  0.8f, m_positions, m_scales, img_KittenMoveR);
    Animation Kitten_L_move   ("kitten_L_move",    true,  0.8f, m_positions, m_scales, img_KittenMoveL);

    std::vector<AnimationController::Transition> transitions;

    animationController.addState("kitten_R_default", Kitten_R_default, transitions);
    animationController.addState("kitten_L_default", Kitten_L_default, transitions);
    animationController.addState("kitten_R_move", Kitten_R_move, transitions);
    animationController.addState("kitten_L_move", Kitten_L_move, transitions);

    player.setAnimationController(animationController);
}

void HandleCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    GetClientRect(hWnd, &rt);
    if (FAILED(image.Load(TEXT("Horror_background.jpg")))) {
        MessageBox(hWnd, TEXT("Failed to load image"), TEXT("Error"), MB_OK);
        PostQuitMessage(0);
    }
    SetTimer(hWnd, 1, gameTick, NULL);
    InitializeAnimations();  // 애니메이션 초기화 호출
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

    floor1.DrawObject3D(mDC, camera);
    floor2.DrawObject3D(mDC, camera);
    floor2.DrawObject3D(mDC, camera);


    wall.DrawObject3D(mDC, camera);
    wall2.DrawObject3D(mDC, camera);
    wall_.DrawObject3D(mDC, camera);
    player.DrawObject3D(mDC, camera);

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
    if (keyStates['A']&& player.getAnimationController().getCurrentState() != "kitten_L_move") {
        player.getAnimationController().setCurrentState("kitten_L_move");
    }
    else if (keyStates['D'] && player.getAnimationController().getCurrentState() != "kitten_R_move") {
        player.getAnimationController().setCurrentState("kitten_R_move");
    }
}

void HandleKeyUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    keyStates[wParam] = false;
    if (KEY_UP_CONDITION('A')) {
        player.getAnimationController().setCurrentState("kitten_L_default");
    }
    else if (KEY_UP_CONDITION('D')) {
        player.getAnimationController().setCurrentState("kitten_R_default");
    }
}

void HandleTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    DWORD currentTime = timeGetTime();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

     // 항상 업데이트

    if (keyStates['A']) {
        player.move2DPosition(-0.1f, 0);
        player.getAnimationController().update(deltaTime);
    }
    if (keyStates['D']) {
        player.move2DPosition(0.1f, 0);
        player.getAnimationController().update(deltaTime);

    }
    if (keyStates['W']) {
        player.move2DPosition(0, 0.1f);
    }
    if (keyStates['S']) {
        player.move2DPosition(0, -0.1f);
    }

    // 카메라가 플레이어를 부드럽게 따라오도록 조작
    Vector3 playerPos = player.getPosition();
    Vector3 cameraPos = camera.getPosition();
    Vector3 targetPos = { playerPos.x, playerPos.y + 3.0f, playerPos.z - 5.5f }; // 카메라의 y좌표를 플레이어보다 낮게 설정

    camera.setPosition({
        cameraPos.x + (targetPos.x - cameraPos.x) * cameraFollowSpeed,
        cameraPos.y + (targetPos.y - cameraPos.y) * cameraFollowSpeed,
        cameraPos.z + (targetPos.z - cameraPos.z) * cameraFollowSpeed
        });

    InvalidateRect(hWnd, NULL, FALSE);
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
