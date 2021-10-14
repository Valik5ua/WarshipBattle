#pragma once
#include <Windows.h>
#include "Deck.h"

class Ship
{
public:
	Ship(const POINT Position, const size_t Size);
	Ship() { this->Size = 0; }
public:
	void SetDamageToDeck(POINT CoordinatesOfDeck);
public:
	bool Deployed;
	bool Deployable;

	bool Rotated;
	bool Killed;

	int Size;

	Deck Decks[4];
};
