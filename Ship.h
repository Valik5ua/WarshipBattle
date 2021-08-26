#pragma once
#include <Windows.h>
#include "Deck.h"

class Ship
{
public:
	Ship(POINT StartPos);
	Ship();
	//bool Click(POINT Coordinates);
	virtual int GetSize() = 0;
public:
	POINT StartPos;
	bool Deployed;
	bool Deployable;
protected:
	int Size;
};
