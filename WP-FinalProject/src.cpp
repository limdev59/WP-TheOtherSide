#include <iostream>
#include <windows.h>
#include <atlimage.h>
#include <mmsystem.h>
#include <algorithm>
#include <vector>
#include "Camera.h"
#include "Object3D.h"
#include "Construction.h"
#include "Animation.h"
#include "Actor.h"
#include "AnimationController.h"
#include "Player.h"
#include "Mouse.h"
#include "Shadow.h"
#pragma comment(lib, "winmm.lib")
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#define KEY_UP_CONDITION(KEY) (!keyStates[KEY] && KEY == wParam)

// 전역 상수
constexpr int gameTick = 7;
constexpr float cameraFollowSpeed = 0.2f; // 카메라가 플레이어를 따라오는 속도
constexpr COLORREF FLOOR_OUTLINE_COLORREF = RGB(42, 32, 50);//RGB(34, 15, 33);
constexpr COLORREF FLOOR_INBRUSH_COLORREF = RGB(42, 32, 50);
constexpr COLORREF WALL_OUTLINE_COLORREF = RGB(24, 24, 40);
constexpr COLORREF WALL_INBRUSH_COLORREF = RGB(24, 15, 33);

// 전역 변수
bool keyStates[256] = { 0 };
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program";

static HDC hDC, mDC;
static PAINTSTRUCT ps;
static HBITMAP hBitmap;
static RECT rt;

std::vector<Construction> walls = {
	//위벽
	{{ 12.5, 4, 67.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 2.5, 4, 37.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 22.5, 4, 7.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 32.5, 4, 7.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 42.5, 2, -2.5 }, { 10, 4, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 42.5, 4, -2.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 42.5, 4, -2.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 42.5, 4, -2.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 22.5, 4, 37.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 32.5, 4, 37.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 42.5, 4, 37.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 52.5, 4, 37.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 62.5, 4, 37.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 72.5, 4, 37.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 92.5, 4, 37.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 102.5, 4, 37.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 82.5, 4, 67.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 112.5, 4, 57.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},

	//아래벽
	{{ 2.5, 4, 27.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 12.5, 4, -2.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 22.5, 4, -2.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 32.5, 4, -2.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 42.5, 4, -2.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 22.5, 4, 27.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 32.5, 4, 27.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 52.5, 4, 27.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 62.5, 4, 27.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 72.5, 4, 27.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 82.5, 4, 27.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 92.5, 4, 27.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 112.5, 4, 27.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 102.5, 4, 17.5 }, { 10, 8, 0 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	//세로벽
	{{ 7.5, 4, 62.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 7.5, 4, 52.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 7.5, 4, 42.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ -2.5, 4, 32.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 7.5, 4, 22.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 7.5, 4, 12.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 7.5, 4, 2.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 17.5, 4, 62.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 17.5, 4, 52.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 17.5, 4, 42.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 17.5, 4, 22.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 17.5, 4, 12.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 37.5, 4, 12.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 37.5, 4, 22.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 47.5, 4, 2.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 47.5, 4, 12.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 47.5, 4, 22.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 77.5, 4, 42.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 77.5, 4, 52.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 77.5, 4, 62.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 87.5, 4, 42.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 87.5, 4, 52.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 87.5, 4, 62.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 107.5, 4, 42.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 107.5, 4, 52.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 117.5, 4, 22.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 117.5, 4, 32.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 117.5, 4, 42.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF},
	{{ 117.5, 4, 52.5 }, { 0, 8, 10 }, WALL_OUTLINE_COLORREF ,WALL_INBRUSH_COLORREF}
};
std::vector<Construction> floors = {
	{{ 10, 0, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 10, 0, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{{ 10, 0, 10 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 10 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 10, 0, 15 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 15 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{{ 10, 0, 20 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 20 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 10, 0, 25 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 25 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{{ 10, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 10, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{{ 10, 0, 40 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 40 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 10, 0, 45 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 45 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{{ 10, 0, 50 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 50 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 10, 0, 55 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 55 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{{ 10, 0, 60 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 60 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 10, 0, 65 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 15, 0, 65 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{{ 0, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 5, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 0, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 5, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 20, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 25, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 20, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 25, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 30, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 35, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 30, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 35, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 20, 0, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 25, 0, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 20, 0, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 25, 0, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 30, 0, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 35, 0, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 30, 0, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 35, 0, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 40, 0, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 45, 0, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 40, 0, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 45, 0, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 40, 0, 10 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 45, 0, 10 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 40, 0, 15 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 45, 0, 15 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 40, 0, 20 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 45, 0, 20 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 40, 0, 25 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 45, 0, 25 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 40, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 45, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 40, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 45, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 50, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 55, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 50, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 55, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 60, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 65, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 60, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 65, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 70, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 75, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 70, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 75, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 80, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 85, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 80, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 85, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 90, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 95, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 90, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 95, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 100, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 105, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 100, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 105, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 110, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 115, 0, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 110, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 115, 0, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 110, 0, 40 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 115, 0, 40 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 110, 0, 45 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 115, 0, 45 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 110, 0, 50 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 115, 0, 50 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 110, 0, 55 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 115, 0, 55 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},

	{{ 100, 0, 20 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 105, 0, 20 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 100, 0, 25 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF},
	{{ 105, 0, 25 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{{ 80, 0, 40 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 85, 0, 40 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 80, 0, 45 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 85, 0, 45 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{{ 80, 0, 50 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 85, 0, 50 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 80, 0, 55 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 85, 0, 55 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{{ 80, 0, 60 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 85, 0, 60 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 80, 0, 65 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{{ 85, 0, 65 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
};
std::vector<Construction> ceilings = {
	{ { 10, 8, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 10, 8, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 10, 8, 10 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 10 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 10, 8, 15 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 15 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 10, 8, 20 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 20 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 10, 8, 25 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 25 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 10, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 10, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 10, 8, 40 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 40 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 10, 8, 45 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 45 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 10, 8, 50 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 50 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 10, 8, 55 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 55 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 10, 8, 60 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 60 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 10, 8, 65 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 15, 8, 65 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 0, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 5, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 0, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 5, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 20, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 25, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 20, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 25, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 30, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 35, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 30, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 35, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 20, 8, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 25, 8, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 20, 8, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 25, 8, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 30, 8, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 35, 8, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 30, 8, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 35, 8, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 40, 8, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 45, 8, 0 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 40, 8, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 45, 8, 5 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 40, 8, 10 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 45, 8, 10 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 40, 8, 15 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 45, 8, 15 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 40, 8, 20 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 45, 8, 20 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 40, 8, 25 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 45, 8, 25 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 40, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 45, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 40, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 45, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },


	{ { 50, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 55, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 50, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 55, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 60, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 65, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 60, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 65, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 70, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 75, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 70, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 75, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 80, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 85, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 80, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 85, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 90, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 95, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 90, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 95, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 100, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 105, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 100, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 105, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 110, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 115, 8, 30 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 110, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 115, 8, 35 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 110, 8, 40 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 115, 8, 40 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 110, 8, 45 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 115, 8, 45 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 110, 8, 50 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 115, 8, 50 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 110, 8, 55 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 115, 8, 55 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 100, 8, 20 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 105, 8, 20 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 100, 8, 25 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 105, 8, 25 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 80, 8, 40 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 85, 8, 40 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 80, 8, 45 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 85, 8, 45 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 80, 8, 50 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 85, 8, 50 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 80, 8, 55 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 85, 8, 55 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },

	{ { 80, 8, 60 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 85, 8, 60 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 80, 8, 65 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF },
	{ { 85, 8, 65 }, { 5, 0, 5 }, FLOOR_INBRUSH_COLORREF, FLOOR_OUTLINE_COLORREF }
};

static DWORD lastTime = timeGetTime();
static AnimationController animationController("kitten_R_default");
static AnimationController animationController2("shadow_A_default");
static Mouse mouse;

// 초기화된 카메라와 객체
static Camera camera({ 0, 3.6f, 0 }, 0.0f, -0.5f, 0.0f);
static Player player({ 0, 1.3, 30 }, { 2.6f, 2.6f, 0 });
static Shadow shadow{{ 0, 1.3, 30 }, { 2.6f, 2.6f, 0 } };
static CImage image;

// 애니메이션 초기화 함수
void InitializeAnimations() {
	std::map<float, POINT> shadow_positions = {
		{0.0f, {0, 0}}
	};
	std::map<float, POINT> shadow_scales = {
		{0.0f, {1, 1}}
	};

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

	std::map<float, std::string> shadow_imagesKittenR = {
		{0.0f, "kitten_R_default_1"}
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

	Animation Shadow_A_default("shadow_A_default", false, 0.0f, shadow_positions, shadow_scales, shadow_imagesKittenR);

	Animation Kitten_R_default("kitten_R_default", false, 0.0f, d_positions, d_scales, img_imagesKittenR);
	Animation Kitten_L_default("kitten_L_default", false, 0.0f, d_positions, d_scales, img_imagesKittenL);
	Animation Kitten_R_move("kitten_R_move", true, 0.8f, m_positions, m_scales, img_KittenMoveR);
	Animation Kitten_L_move("kitten_L_move", true, 0.8f, m_positions, m_scales, img_KittenMoveL);

	std::vector<AnimationController::Transition> transitions;
	std::vector<AnimationController::Transition> transitions2;

	animationController2.addState("shadow_A_default", Shadow_A_default, transitions2);

	animationController.addState("kitten_R_default", Kitten_R_default, transitions);
	animationController.addState("kitten_L_default", Kitten_L_default, transitions);
	animationController.addState("kitten_R_move", Kitten_R_move, transitions);
	animationController.addState("kitten_L_move", Kitten_L_move, transitions);

	player.setAnimationController(animationController);
	shadow.setAnimationController(animationController2);
}

void CALLBACK HandleCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	GetClientRect(hWnd, &rt);
	std::sort(walls.begin(), walls.end(), compareByZ);

	if (FAILED(image.Load(TEXT("Horror_background.jpg")))) {
		MessageBox(hWnd, TEXT("Failed to load image"), TEXT("Error"), MB_OK);
		PostQuitMessage(0);
	}
	SetTimer(hWnd, 1, gameTick, NULL);
	InitializeAnimations();
}

void CALLBACK HandleResize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	GetClientRect(hWnd, &rt);
	DeleteObject(hBitmap);
	hBitmap = CreateCompatibleBitmap(hDC, rt.right, rt.bottom);
	SelectObject(mDC, hBitmap);
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

	for (auto& floor : floors) {
		floor.DrawObject3D(mDC, camera);
	}
	for (auto& ceiling : ceilings) {
		ceiling.DrawObject3D(mDC, camera);
	}
	for (auto& wall : walls) {
		wall.DrawObject3D(mDC, camera);
	}

	shadow.DrawObject3D(mDC, camera);
	player.DrawObject3D(mDC, camera);

	BitBlt(hDC, 0, 0, rt.right, rt.bottom, mDC, 0, 0, SRCCOPY);
	DeleteDC(mDC);
	DeleteObject(hBitmap);
	EndPaint(hWnd, &ps);
}

// 콜백 함수들
void CALLBACK HandleLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	mouse.OnMouseDown(wParam, lParam);
	shadow.setLeftDown(true);
}

void CALLBACK HandleLButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	mouse.OnMouseUp(wParam, lParam);
	shadow.setLeftDown(false);
	Vector3 a = (Unproject2DTo3D(camera, { mouse.getMousePosition().x + 10, mouse.getMousePosition().y }, 1.3f) - player.getPosition());
	shadow.setDirection(a);
	std::cout << shadow.getPosition().x << ' ' << shadow.getPosition().y << ' ' << shadow.getPosition().z << std::endl;
}

void CALLBACK HandleRButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	mouse.OnMouseDown(wParam, lParam);
	shadow.setRightDown(true);
}

void CALLBACK HandleRButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	mouse.OnMouseUp(wParam, lParam);
	shadow.setRightDown(false);
}

void CALLBACK HandleMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	mouse.OnMouseMove(lParam);
}

void CALLBACK HandleKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	keyStates[wParam] = true;
	if (keyStates['A'] && player.getAnimationController().getCurrentState() != "kitten_L_move") {
		player.getAnimationController().setCurrentState("kitten_L_move");
	}
	else if (keyStates['D'] && player.getAnimationController().getCurrentState() != "kitten_R_move") {
		player.getAnimationController().setCurrentState("kitten_R_move");
	}
}

void CALLBACK HandleKeyUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	keyStates[wParam] = false;
	if (KEY_UP_CONDITION('A')) {
		player.getAnimationController().setCurrentState("kitten_L_move");
	}
	else if (KEY_UP_CONDITION('D')) {
		player.getAnimationController().setCurrentState("kitten_R_move");
	}
}

void CALLBACK HandleTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	DWORD currentTime = timeGetTime();
	float deltaTime = (currentTime - lastTime) / 1000.0f;
	lastTime = currentTime;

	bool cantMoveLeft = true;
	bool cantMoveRight = true;
	bool cantMoveUp = true;
	bool cantMoveDown = true;

	//각 바닥과의 충돌 검사
	for (const Construction& floor : floors) {
		POINT playerPos = player.get2DPosition();
		Vector3 pos = floor.getPosition();
		Vector3 size = floor.getSize();
		RECT bound = {
			static_cast<LONG>(pos.x - size.x / 2),
			static_cast<LONG>(pos.z - size.z / 2),
			static_cast<LONG>(pos.x + size.x / 2),
			static_cast<LONG>(pos.z + size.z / 2)
		};

		// 왼쪽 충돌 검사
		if (playerPos.x - 0.2f >= bound.left && playerPos.x <= bound.right && playerPos.y >= bound.top && playerPos.y <= bound.bottom) {
			cantMoveLeft = false;
		}
		// 오른쪽 충돌 검사
		if (playerPos.x + 0.2f <= bound.right && playerPos.x >= bound.left && playerPos.y >= bound.top && playerPos.y <= bound.bottom) {
			cantMoveRight = false;
		}
		// 위쪽 충돌 검사
		if (playerPos.y - 0.2f >= bound.top && playerPos.y <= bound.bottom && playerPos.x >= bound.left && playerPos.x <= bound.right) {
			cantMoveDown = false;
		}
		// 아래쪽 충돌 검사
		if (playerPos.y + 0.2f <= bound.bottom && playerPos.y >= bound.top && playerPos.x >= bound.left && playerPos.x <= bound.right) {
			cantMoveUp = false;
		}
	}
	// shadow 조작
	{
		if (keyStates[VK_SPACE]) {
			std::string st = player.getAnimationController().getCurrentState();
			Vector3 playerPos = player.getPosition();
			Vector3 shadowPos = shadow.getPosition();
			Vector3 targetPos = {
				playerPos.x + ((st == "kitten_R_default" || st == "kitten_R_move") ? -0.0f : 0.0f),
				playerPos.y,
				playerPos.z
			};
			shadow.setPosition({
				shadowPos.x + (targetPos.x - shadowPos.x) * cameraFollowSpeed,
				shadowPos.y + (targetPos.y - shadowPos.y) * cameraFollowSpeed,
				shadowPos.z + (targetPos.z - shadowPos.z) * cameraFollowSpeed
				});
		}

		//mouse.getMouse3DPosition(camera);
		shadow.update(deltaTime, mouse.getMousePosition());
		

		shadow.getAnimationController().setCurrentState("shadow_A_default");
		shadow.getAnimationController().update(deltaTime);
	}
	// player, camera 움직임 처리
	{
		if (keyStates['A'] || keyStates['D'] || keyStates['W'] || keyStates['S']) {
			player.getAnimationController().update(deltaTime);
		}
		if (!keyStates[VK_SPACE]) {
			if (keyStates['A']) {
				if (!cantMoveLeft) {
					player.move2DPosition(-0.2f, 0);
					if (keyStates[VK_SHIFT]) player.move2DPosition(-1.0f, 0);
				}
			}
			if (keyStates['D']) {
				if (!cantMoveRight) {
					player.move2DPosition(0.2f, 0);
					if (keyStates[VK_SHIFT]) player.move2DPosition(1.0f, 0);
				}
			}
			if (keyStates['W']) {
				if (!cantMoveUp) {
					player.move2DPosition(0, 0.2f); // 일반 이동
					if (keyStates[VK_SHIFT]) player.move2DPosition(0, 1.0f); // 쉬프트 키 누르면 빠른 이동
				}
			}
			if (keyStates['S']) {
				if (!cantMoveDown) {
					player.move2DPosition(0, -0.2f); // 일반 이동
					if (keyStates[VK_SHIFT]) player.move2DPosition(0, -1.0f); // 쉬프트 키 누르면 빠른 이동
				}
			}
		}
		// camera가 플레이어를 부드럽게 따라오도록 조작
		{
			Vector3 playerPos = player.getPosition();
			Vector3 cameraPos = camera.getPosition();
			Vector3 targetPos = {
				playerPos.x + (keyStates['A'] && !cantMoveLeft ? -1.0f : (keyStates['D'] && !cantMoveRight ? 1.0f : 0.0f)),
				playerPos.y + 3.0f + (keyStates['W'] ? 0.1f : (keyStates['S'] ? -0.1f : 0.0f)),
				playerPos.z - 5.8f + (keyStates['W'] ? 0.5f : (keyStates['S'] ? -1.5f : 0.0f))
			};
			camera.setPosition({
				cameraPos.x + (targetPos.x - cameraPos.x) * cameraFollowSpeed,
				cameraPos.y + (targetPos.y - cameraPos.y) * cameraFollowSpeed,
				cameraPos.z + (targetPos.z - cameraPos.z) * cameraFollowSpeed
				});
			float imsi = -0.5f;
			Vector3 cameraRot = camera.getRotation();
			Vector3 targetRot = {
				cameraRot.x,
				(keyStates['W'] ? imsi + 0.1f : (keyStates['S'] ? imsi - 0.1f : imsi)),
				(keyStates['A'] ? 0.025f : (keyStates['D'] ? -0.025f : 0.0f))
			};
			camera.setRotation({
				cameraRot.x + (targetRot.x - cameraRot.x) * cameraFollowSpeed,
				cameraRot.y + (targetRot.y - cameraRot.y) * cameraFollowSpeed,
				cameraRot.z + (targetRot.z - cameraRot.z) * cameraFollowSpeed
				});
		}
	}

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
	case WM_RBUTTONDOWN:
		HandleRButtonDown(hWnd, uMsg, wParam, lParam);
		return 0;
	case WM_RBUTTONUP:
		HandleRButtonUp(hWnd, uMsg, wParam, lParam);
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
