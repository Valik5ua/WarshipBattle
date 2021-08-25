#pragma once
#include "Cell.h"

class Deck : public Cell
{
public:
	GLint TextureID;
	GLint TextureID_Direction;
	enum Status { Water, Ship_Whole, Ship_Afire, Ship_Cracked, Missed };
	Status Stat;
};
