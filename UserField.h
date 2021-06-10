#pragma once
#include "Field.h"

extern const int MyGameFieldW;
extern const int MyGameFieldH;

class UserField : public Field
{
public:
	UserField(int x, int y)
	{
		this->StartX = x;
		this->StartY = y;
	}
	bool Click(POINT& coordinates) override;
	void Draw() override;
	void Select(const size_t, const size_t) override;
	void Deselect() override;
	Cell Cells[MyGameFieldW][MyGameFieldH];
};
