#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"

class Cell
{
public:
	Cell();
	void Draw(POINT Coordinates, GLuint TextureID, bool Rotated = false);
public:
	bool MarkedShip;
	bool Cell_Aim;
	bool Open;
	bool Missed;
	int ButtonID;
};
