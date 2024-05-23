#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <sstream>
#include "resource.h"

//void HandleCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HandlePaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//void HandleResize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//void HandleMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//void HandleLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//void HandleLButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HandleKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HandleKeyUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//void HandleTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInst;


static HDC hDC, mDC;
static PAINTSTRUCT ps;
static HBITMAP hBitmap;
static RECT rt;

using namespace std;

//¸¶¿ì½º
bool isLeftClick = false;
bool isRightClick = false;
POINT startPosR, endPosR, startPosL, endPosL;

void HandleKeyDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (wParam)
	{
	case VK_LEFT:
		break;
	case VK_UP:
		break;
	case VK_RIGHT:
		break;
	case VK_DOWN:
		break;
	case VK_RETURN:
	default:
		break;
	}
}

void HandleKeyUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (wParam)
	{
	case VK_LEFT:
		break;
	case VK_UP:
		break;
	case VK_RIGHT:
		break;
	case VK_DOWN:
		break;
	case VK_RETURN:
	default:
		break;
	}
}
void HandlePaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	hDC = BeginPaint(hWnd, &ps);
	mDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
	SelectObject(mDC, hBitmap);
	FillRect(mDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));


	BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
	DeleteDC(mDC);
	DeleteObject(hBitmap);
	EndPaint(hWnd, &ps);

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
	case WM_LBUTTONDOWN:
		//HandleLButtonDown(hWnd, uMsg, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		//HandleLButtonUp(hWnd, uMsg, wParam, lParam);
		break;
	case WM_KEYDOWN:
		HandleKeyDown(hWnd, uMsg, wParam, lParam);
		break;
	case WM_KEYUP:
		HandleKeyUp(hWnd, uMsg, wParam, lParam);
		break;
	case WM_TIMER:
		//HandleTimer(hWnd, uMsg, wParam, lParam);
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
	static LPCTSTR lpszClass = L"Window Class Name";
	static LPCTSTR lpszWindowName = L"windows program";
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