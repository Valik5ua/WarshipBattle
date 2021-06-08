#pragma once
#include "Field.h"

extern const int OpponentGameFieldW;
extern const int OpponentGameFieldH;

class EnemyField : public Field
{
public:
	EnemyField(int x, int y)
	{
		this->StartX = x;
		this->StartY = y;
	}
	bool Click(POINT& coordinates) override;
	void Draw()	override;
	Cell Cells[OpponentGameFieldW][OpponentGameFieldH];
};
