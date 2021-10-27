#pragma once
#include "Field.h"
class CannonField : public Field
{
public:
	CannonField(const unsigned int StartX, const unsigned int StartY, float angle) :Field(StartX, StartY), angle(angle) {}
	void Draw() override;
	void Draw(float angle);
private:
	bool Click(POINT& coordinates) override { return false;}
	float angle;
};
