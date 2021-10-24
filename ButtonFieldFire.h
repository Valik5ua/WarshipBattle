#pragma once
#include "Field.h"
#include "Cell.h"

extern unsigned const int ButtonFieldW;
extern unsigned const int ButtonFieldH;

class ButtonFieldFire : public Field
{
public:
	ButtonFieldFire(int StartX, int StartY);
	bool Click(POINT& coordinates) override;
	void Draw() override;
public:
	Cell Cells[ButtonFieldW][ButtonFieldH];
};

