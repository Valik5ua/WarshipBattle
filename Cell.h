#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"

class Cell
{
public:
	Cell();
	void Draw(POINT Coordinates, GLuint TextureID);
public:
	bool MarkedShip;
	bool Cell_Aim;
	bool Open;
	bool Missed;
	int ButtonID;
};
