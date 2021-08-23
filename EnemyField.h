#pragma once
#include "Field.h"
#include "Cell.h"

extern const int OpponentGameFieldW;
extern const int OpponentGameFieldH;

class EnemyField : public Field
{
public:
	EnemyField(int x, int y) :Field(x, y) {}
	bool Click(POINT& coordinates) override;
	void Draw()	override;
	void Select(const size_t, const size_t);
	bool MoveSelection(int Direction);
public:
	Cell Cells[OpponentGameFieldW][OpponentGameFieldH];
private:
	void Deselect();
};
