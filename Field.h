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
	virtual void StartDeploying() {};
	virtual void Draw();
	bool ShipExistsAround(const POINT Coordinates, Ship* ship = (Ship*)NULL);
	bool ShipExistsAround(Ship&);
	virtual void DrawShip(Ship ship) {};
public:
	int StartX;
	int StartY;
};
