#pragma once
#include "Field.h"
#include "Cell.h"

extern unsigned const int ButtonFieldW;
extern unsigned const int ButtonFieldH;

class ButtonFieldDeploy : public Field
{
public:
	ButtonFieldDeploy(int StartX, int StartY);
	bool Click(POINT& coordinates) override;
	void Draw() override;
	void Deploy();
public:
	Cell Cells[ButtonFieldW][ButtonFieldH];
};
