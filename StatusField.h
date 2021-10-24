#pragma once
#include "Field.h"

class StatusField : public Field
{
public:
	StatusField(const unsigned int StartX, const unsigned int StartY) :Field(StartX, StartY) {}
	void Draw() override;
	bool Click(POINT& coordinates) override { return false; }
};
