#pragma once
#include "Field.h"

class ClueField : public Field
{
public:
	ClueField(const unsigned int StartX, const unsigned int StartY);
	void Draw() override;
	bool Click(POINT& coordinates) override { return false; }
public:
	float startX;
	float startY;
};
