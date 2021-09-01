#include "UserField.h"
#include "Engine.h"
#include "TextureManager.h"

extern TextureManager textureManager;
extern Ship* Ships[10];
extern Engine engine;

/// <summary>
/// Changes OpenGL coordinates to UserField coordinates.
/// </summary>
/// <param name="coordinates: ">The coordinates of the click to be converted.</param>
/// <returns>Wether or not the user has clicked on the UserField.</returns>
bool UserField::Click(POINT& coordinates)
{
	if (coordinates.x >= this->StartX && coordinates.y >= this->StartY && coordinates.x < MyGameFieldW + this->StartX && coordinates.y < MyGameFieldH + this->StartY)
	{
		coordinates.x -= this->StartX;
		coordinates.y -= this->StartY;
		return true;
	}
	return false;
}

void UserField::StartDeploying()
{
	DrawShip(this->Ships[0]);
}

void UserField::MoveActiveShip(int Direction)
{
	switch (Direction)
	{
	case BF_MOVE_LEFT:
		this->Ships[engine.ShipsDeployed].StartPos.x += 1;
		break;
	case BF_MOVE_RIGHT:
		this->Ships[engine.ShipsDeployed].StartPos.x -= 1;
		break;
	case BF_MOVE_UP:
		this->Ships[engine.ShipsDeployed].StartPos.y += 1;
		break;
	case BF_MOVE_DOWN:
		this->Ships[engine.ShipsDeployed].StartPos.y -= 1;
		break;
	}
}
