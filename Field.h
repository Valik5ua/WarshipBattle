#pragma once
#include "Cell.h"
#include "resource.h"

extern HWND hwnd;

class Field
{
public:
	Field();
	virtual bool Click(POINT& coordinates) = 0;
	virtual void Draw() = 0;
	virtual void Select(const size_t, const size_t) = 0;
	virtual void Deselect();
	int StartX;
	int StartY;
};
