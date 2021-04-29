#pragma once
#include <windows.h>

// Program Metrics
#define MINIMAL_WIDTH           1050 
#define MINIMAL_HEIGHT          600

// Windows globals
CHAR WindowClassName[] = { "Windows OpenGL" };
HWND  hwnd;
HDC   hDC;
HGLRC hRC;

// Custom globals
bool TimeToRedraw;
HANDLE TimerFuncHandler;
float FrameRate = (float)1000 / 60;