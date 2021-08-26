#pragma once
#include "Ship.h"

class TwoDeckShip : public Ship
{
public:
	TwoDeckShip() { StartPos = { 0,0 }; Size = 2; }
	TwoDeckShip(POINT StartPos) { this->StartPos = StartPos; Size = 2; };
	int GetSize() { return Size; };
public:
	Deck* deck = new Deck[Size];
};
