#pragma once
#include "Field.h"
#include "Cell.h"
#include "Engine.h"

extern const int OpponentGameFieldW;
extern const int OpponentGameFieldH;

class EnemyField : public Field
{
public:
	EnemyField(int x, int y) :Field(x, y), DeployingShipID(0) {}
	bool Click(POINT& coordinates) override;
	void Select(const size_t, const size_t);
	void RandomSelect(const size_t, const size_t);
	bool MoveSelection(int Direction);

	int ShipExists(const POINT Coordinates);
	void CreateShips(const Engine::GAMESTATUS);
	void CloseNextShip();
	
	void SetShipMarkers();
	void Draw() override;
private:
	void SetShipDeployableStatus(Ship& ship);
	void DeployEnemyShips();
	void ClearField();
	void CleanShips();
	void Deselect();
public:
	Cell Cells[OpponentGameFieldW][OpponentGameFieldH];
	Ship Ships[10];
private:
	unsigned int DeployingShipID;
};
