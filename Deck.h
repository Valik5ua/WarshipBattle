#pragma once
#include "Cell.h"
#include "resource.h"

class Deck : public Cell
{
public:
	Deck() :Cell(), integrityStatus(IntegrityStatus::Whole) {};

	POINT Position;

	enum DeckType { Front, Back, Middle, Single };
	DeckType Type;

	enum IntegrityStatus { Whole, Damaged, Killed };
	IntegrityStatus integrityStatus;
};
