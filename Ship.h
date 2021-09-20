#pragma once
#include <Windows.h>
#include "Deck.h"

class Ship
{
public:
	Ship(POINT Position, int Size);
	Ship() {};
	void SetDamageToDeck(int Coordinates);
public:
	bool Deployed;
	bool Deployable;

	bool Rotated;
	bool Killed;

	int Size;

	Deck Decks[4];
};
