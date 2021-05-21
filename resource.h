#pragma once
#include "Engine.h"

// Bottom level Menus For New Game
#define MENU_GAME_PVP			1

#define MENU_GAME_EASY			11
#define MENU_GAME_MEDIUM		12
#define MENU_GAME_HARD			13

#define MENU_GAME_EXIT			101

//Bottom level Menu For Help
#define MENU_HELP_ABOUT			201

// Program Metrics
const float MinSizeFactor = 30;

const float OpenGLWidth = 32.0;
const float OpenGLHeight = 16.0;
const float AspectRatio = OpenGLWidth / OpenGLHeight;
const float MinimalWidth = OpenGLWidth * MinSizeFactor;
const float MinimalHeight = OpenGLHeight * (MinSizeFactor + 3.3);

// Windows globals
extern CHAR   WindowClassName[];
extern HWND   hwnd;
extern HDC    hDC;
extern HGLRC  hRC;

// Custom globals
extern bool   TimeToRedraw;
extern HANDLE TimerFuncHandler;
extern float  FrameRate;
extern Engine _Engine;
