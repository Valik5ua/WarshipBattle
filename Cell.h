#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"

class Cell
{
public:
	Cell();
public:
	enum Status { Water, Ship_Whole, Ship_Afire, Ship_Cracked, Missed };
	Status Stat;
	bool Cell_Aim;
	bool Selected;
	int ButtonID;
};
