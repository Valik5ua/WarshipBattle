#pragma once
#include "Field.h"
#include "Cell.h"

class ButtonFieldConnect : public Field
{
public:
	ButtonFieldConnect(int StartX, int StartY);
	bool Click(POINT& coordinates) override;
	void Draw() override;
	void Select(const size_t, const size_t) override {};
private:
	void Deselect() override {};
public:
	Cell Cells[ButtonFieldW][ButtonFieldH];
};
