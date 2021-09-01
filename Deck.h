#pragma once
#include "Cell.h"
#include "resource.h"

class Deck : public Cell
{
public:
	Deck() {};

	bool Rotated;

	enum DeckType { Front, Back, Middle, Single };
	DeckType Type;

	enum IntegrityStatus { Whole, Damaged, Killed };
	IntegrityStatus integrityStatus;
};
