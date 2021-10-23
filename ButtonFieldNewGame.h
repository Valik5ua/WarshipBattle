#pragma once
#include "Field.h"

extern unsigned const int ButtonFieldW;
extern unsigned const int ButtonFieldH;

class ButtonFieldNewGame : public Field
{
public:
	ButtonFieldNewGame(const int StartX, const int StartY);
	bool Click(POINT& coordinates) override;
	void Draw() override;
public:
	Cell Cells[ButtonFieldW][ButtonFieldH];
};
