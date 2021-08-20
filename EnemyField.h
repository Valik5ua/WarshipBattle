#pragma once
#include "Field.h"
#include "Cell.h"

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
	void Select(const size_t, const size_t) override;
	void Deselect() override;
	bool MoveSelection(int Direction);
	Cell Cells[OpponentGameFieldW][OpponentGameFieldH];
};
