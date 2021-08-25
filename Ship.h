#pragma once
#include <Windows.h>
#include "Deck.h"

class Ship
{
public:
	Ship(POINT StartPos);
	Ship();
	virtual bool Click(POINT Coordinates) = 0;
public:
	POINT StartPos;
	bool Deployed;
	bool Deployable;
private:
	int Size;
};
