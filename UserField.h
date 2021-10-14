#pragma once
#include "Field.h"

extern const int UserGameFieldW;
extern const int UserGameFieldH;

class UserField : public Field
{
public:
	UserField(int x, int y) :Field(x, y), AimPoint({5,5}) {}
	bool Click(POINT& coordinates) override;
	
	void SwapActiveShip(const unsigned int ShipNum);
	int ShipExists(POINT Coordinates, unsigned const int ShipsToCheck);
	void SetShipDeployableStatus();
	void MoveActiveShip(int Direction);
	void RotateActiveShip();
	
	void SetShipMarkers();
	void Draw() override;

	void ClearField();
	void CleanShips();

	void SetAimPoint(POINT AimPoint);

	bool CanFire() override { return true; }
	int ShootRecieve(const POINT ShootCoordinates) override;
	POINT ShootCreate() override;
	void ShootAnswer(const int status) override;
private:
	bool In_Range(POINT Coordinates);
public:
	Cell Cells[UserGameFieldW][UserGameFieldH];
	Ship Ships[10];
private:
	POINT AimPoint;
};
