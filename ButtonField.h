#pragma once
#include "Field.h"
#include "Cell.h"

extern const int ButtonFieldW;
extern const int ButtonFieldH;

class ButtonField : public Field
{
public:
	ButtonField(int StartX, int StartY);
	bool Click(POINT& coordinates) override;
	void Draw() override;
	void Select(const size_t, const size_t) override;
private:
	void Deselect();
public:
	Cell Cells[ButtonFieldW][ButtonFieldH];
};
