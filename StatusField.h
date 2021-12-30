#pragma once
#include "Field.h"

class StatusField : public Field
{
public:
	StatusField(const float startX, const float startY) :startX(startX), startY(startY) {}
	void Draw() override;
	bool Click(POINT& coordinates) override { return false; }
public:
	float startX;
	float startY;
};
