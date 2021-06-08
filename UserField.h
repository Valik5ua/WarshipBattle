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
	Cell Cells[MyGameFieldW][MyGameFieldH];
};
