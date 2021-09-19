#pragma once
#include "Field.h"

extern const int UserGameFieldW;
extern const int UserGameFieldH;

class UserField : public Field
{
public:
	UserField(int x, int y) :Field(x,y) {}
	bool Click(POINT& coordinates) override;
	
	void SwapActiveShip(const unsigned int ShipNum);
	int ShipExists(POINT Coordinates);
	void SetShipDeployableStatus();
	void MoveActiveShip(int Direction);
	void RotateActiveShip();
	
	void SetShipMarkers();
	void Draw() override;
private:
	bool In_Range(POINT Coordinates);
public:
	Cell Cells[UserGameFieldW][UserGameFieldH];
	Ship Ships[10];
};
