#pragma once
#include "Field.h"

const int OpponentGameFieldW = 10;
const int OpponentGameFieldH = 10;

class EnemyField : public Field
{
public:
	EnemyField(int x, int y)
	{
		StartX = x;
		StartY = y;
	}
	bool Click(POINT coordinates) override;
	void Draw()	override;
	Cell Cells[OpponentGameFieldW][OpponentGameFieldH];
};
