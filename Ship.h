#pragma once
#include <Windows.h>
#include "Deck.h"

class Ship
{
public:
	Ship(POINT StartPos, int Size);
	Ship() {};
	void SetSize(int Size);
	//bool Click(POINT Coordinates);
	void SetDamageToDeck(int Coordinates);
public:
	POINT StartPos;
	bool Deployed;
	bool Deployable;
	bool Killed;
	int Size;
	Deck Decks[4];
};
