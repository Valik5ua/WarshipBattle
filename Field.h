#pragma once
#include <windows.h>
#include "resource.h"

extern HWND hwnd;

class Field
{
public:
	Field();
	Field(int StartX, int StartY)
	{
		this->StartX = StartX;
		this->StartY = StartY;
	};
	virtual bool Click(POINT& coordinates) = 0;
	virtual void Draw() = 0;
	int StartX;
	int StartY;
};
