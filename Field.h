#pragma once
#include "resource.h"
#include <windows.h>
#include "Ship.h"

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
	bool ShipExistsAround(Cell);
	bool ShipExistsAround(Ship);
public:
	int StartX;
	int StartY;
};
