#pragma once
#include "Ship.h"

extern HWND hwnd;

class Field
{
public:
	Field();
	Field(const unsigned int StartX, const unsigned int StartY)
	{
		this->StartX = StartX;
		this->StartY = StartY;
	};
	virtual bool Click(POINT& coordinates) = 0;
	virtual void Draw() = 0;
	virtual int ShootRecieve(const POINT ShootCoordinates) { return NULL; }
	virtual POINT ShootCreate() { return POINT(); }
	virtual void ShootAnswer(const int status) {}
	virtual bool CanFire() { return true; }
public:
	int StartX;
	int StartY;
};
