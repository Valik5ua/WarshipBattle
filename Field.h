#pragma once
#include "Cell.h"
#include <map>

extern HWND hwnd;

class Field
{
public:
	Field();
	virtual bool Click(float GLmouseX, float GLmouseY) { MessageBox(hwnd, L"Click in base class field (from field.h) .", L"Clicked on Field", NULL); return false; }
	virtual void Draw() {}
	int StartX;
	int StartY;
};
