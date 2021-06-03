#pragma once
#include "Field.h"

const int MyGameFieldW = 4;
const int MyGameFieldH = 10;

class UserField : public Field
{
public:
	UserField(int x, int y)
	{
		StartX = x;
		StartY = y;
	}
	bool Click(float GLmouseX, float GLmouseY);
	void Draw();
	Cell Cells[MyGameFieldW][MyGameFieldH];
};
