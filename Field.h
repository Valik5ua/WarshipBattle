#pragma once
#include "Cell.h"

class Field : public Cell
{
public:
	Field(bool DV,bool DH);
	bool Click(float x, float y);
	void Draw();
private:
	bool Draw_Vertical;
	bool Draw_Horizontal;
};
