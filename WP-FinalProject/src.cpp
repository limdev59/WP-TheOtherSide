#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <sstream>
#include "resource.h"
#include "Mouse.h"

void HandleCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HandlePaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//void HandleResize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//void HandleMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//void HandleLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//void HandleLButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//void HandleKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//void HandleKeyUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HandleTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program";


static HDC hDC, mDC;
static PAINTSTRUCT ps;
static HBITMAP hBitmap;
static RECT rt;
static UINT_PTR timerId = 0;

static Mouse mouse;

using namespace std;


void HandleCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	timerId = SetTimer(hWnd, 1, 16, NULL);
}
void HandlePaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	GetClientRect(hWnd, &rt);
	hDC = BeginPaint(hWnd, &ps);
	GetClientRect(hWnd, &rt);
	mDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
	SelectObject(mDC, hBitmap);
	FillRect(mDC, &rt, (HBRUSH)GetStockObject(GRAY_BRUSH));

	POINT test[4] = {
		{0, 0}, // 왼쪽 위
		{300, 50}, // 오른쪽 위
		{600, 300}, // 오른쪽 아래
		{100, 600}  // 왼쪽 아래
	};
	SelectObject(mDC, (HPEN)GetStockObject(BLACK_PEN));
	SelectObject(mDC, (HBRUSH)GetStockObject(BLACK_BRUSH));
	Polygon(mDC, test, 4);

	BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
	DeleteDC(mDC);
	DeleteObject(hBitmap);
	EndPaint(hWnd, &ps);

}
void HandleTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	InvalidateRect(hWnd, NULL, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		//HandleCreate(hWnd, uMsg, wParam, lParam);
		break;
	case WM_PAINT:
		HandlePaint(hWnd, uMsg, wParam, lParam);
		break;
	case WM_SIZE:
		//HandleResize(hWnd, uMsg, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		//HandleMouseMove(hWnd, uMsg, wParam, lParam);
		break;
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		mouse.OnMouseDown(uMsg, lParam);
		//HandleLButtonDown(hWnd, uMsg, wParam, lParam);
		break;
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
		mouse.OnMouseUp(uMsg, lParam);
		//HandleLButtonUp(hWnd, uMsg, wParam, lParam);
		break;
	case WM_KEYDOWN:
		//HandleKeyDown(hWnd, uMsg, wParam, lParam);
		break;
	case WM_KEYUP:
		//HandleKeyUp(hWnd, uMsg, wParam, lParam);
		break;
	case WM_TIMER:
		HandleTimer(hWnd, uMsg, wParam, lParam);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL; //MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 100, 50, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}