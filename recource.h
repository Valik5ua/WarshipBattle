#pragma once
#include <Windows.h>

// Program Metrics
#define MINIMAL_WIDTH           1050 
#define MINIMAL_HEIGHT          600
#define OPENGLWIDTH				25
#define OPENGLHEIGHT			14

// Bottom level Menus For New Game
#define MENU_NEW_PVP			1
#define MENU_PVE_EASY			101
#define MENU_PVE_MEDIUM			102
#define MENU_PVE_HARD			103

//Bottom level Menu For Help
#define MENU_HELP_ABOUT			201

// Windows globals
extern CHAR   WindowClassName[];
extern HWND   hwnd;
extern HDC    hDC;
extern HGLRC  hRC;

// Custom globals
extern bool   TimeToRedraw;
extern HANDLE TimerFuncHandler;
extern float  FrameRate;
