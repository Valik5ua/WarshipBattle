#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"

class Cell
{
public:
	Cell();
public:
	enum Status { opened, closed };
	Status Stat;
};
