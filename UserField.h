#pragma once
#include "Field.h"

extern const int MyGameFieldW;
extern const int MyGameFieldH;

class UserField : public Field
{
public:
	UserField(int x, int y) :Field(x,y) {}
	bool Click(POINT& coordinates) override;
	void Draw() override;
public:
	Cell Cells[MyGameFieldW][MyGameFieldH];
private:
	
};
