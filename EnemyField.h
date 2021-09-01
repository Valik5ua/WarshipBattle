#pragma once
#include "Field.h"
#include "Cell.h"
#include "Engine.h"

extern const int OpponentGameFieldW;
extern const int OpponentGameFieldH;

class EnemyField : public Field
{
public:
	EnemyField(int x, int y) :Field(x, y) {}
	bool Click(POINT& coordinates) override;
	void Select(const size_t, const size_t);
	bool MoveSelection(int Direction);
	void CreateShips(Engine::MODE);
	void CloseShip();
private:
	void ClearField();
	void CleanShips();
public:
	Cell Cells[OpponentGameFieldW][OpponentGameFieldH];
	Ship Ships[10];
private:
	void Deselect();
};
