#pragma once
#include "Field.h"

class StatusField : public Field
{
public:
	StatusField(const unsigned int startX, const unsigned int startY) :startX(startX), startY(startY) {}
	void Draw() override;
	bool Click(POINT& coordinates) override { return false; }
public:
	float startX;
	float startY;
};
