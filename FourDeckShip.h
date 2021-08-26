#pragma once
#include "Ship.h"

class FourDeckShip : public Ship
{
public:
	FourDeckShip() { StartPos = { 0,0 }; Size = 4; }
	FourDeckShip(POINT StartPos) { this->StartPos = StartPos; Size = 4; };
	int GetSize() { return Size; };
public:
	Deck* deck = new Deck[Size];
};
