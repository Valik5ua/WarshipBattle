#pragma once
#include "Ship.h"

class ThreeDeckShip : public Ship
{
public:
	ThreeDeckShip() { StartPos = { 0,0 }; Size = 3; }
	ThreeDeckShip(POINT StartPos) { this->StartPos = StartPos; Size = 3; };
	int GetSize() { return Size; };
public:
	Deck* deck = new Deck[Size];
};
