#pragma once
#include "Ship.h"

class OneDeckShip : public Ship
{
public:
	OneDeckShip() { StartPos = { 0,0 }; Size = 1; }
	OneDeckShip(POINT StartPos) { this->StartPos = StartPos; Size = 1; };
	int GetSize() { return Size; };
public:
	Deck* deck = new Deck[Size];
};
