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
	void RandomSelect(const size_t, const size_t);
	bool MoveSelection(int Direction);

	bool ShipExists(POINT Coordinates);
	void CreateShips(Engine::MODE);
	void CloseNextShip();
	
	void SetShipMarkers();
	void Draw() override;
private:
	void ClearField();
	void CleanShips();
public:
	Cell Cells[OpponentGameFieldW][OpponentGameFieldH];
	Ship Ships[10];
private:
	void Deselect();
};
