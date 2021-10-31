#include "ButtonFieldDeploy.h"
#include "TextureManager.h"
#include "UserField.h"
#include "EnemyField.h"
#include "Engine.h"
#include "resource.h"

extern Engine engine;
extern UserField userField;
extern EnemyField enemyField;
extern TextureManager textureManager;

/// <summary>
/// Default constructor for the ButtonFieldDeploy class.
/// </summary>
/// <param name="StartX">The X coordinate of the ButtonField.</param>
/// <param name="StartY">The Y coordinate of the ButtonField.</param>
ButtonFieldDeploy::ButtonFieldDeploy(int StartX, int StartY)
{
	this->StartX = StartX;
	this->StartY = StartY;
	this->Cells[0][0].ButtonID = BF_MOVE_LEFT;
	this->Cells[0][1].ButtonID = BF_MOVE_LEFT;

	this->Cells[0][2].ButtonID = BF_MOVE_UP;

	this->Cells[1][0].ButtonID = BF_MOVE_DOWN;

	this->Cells[1][1].ButtonID = BF_ROTATE;

	this->Cells[1][2].ButtonID = BF_MOVE_UP;

	this->Cells[2][0].ButtonID = BF_MOVE_DOWN;

	this->Cells[2][1].ButtonID = BF_MOVE_RIGHT;
	this->Cells[2][2].ButtonID = BF_MOVE_RIGHT;

	this->Cells[3][0].ButtonID = BF_DEPLOY;
	this->Cells[3][1].ButtonID = BF_DEPLOY;
	this->Cells[3][2].ButtonID = BF_DEPLOY;
}

/// <summary>
/// Changes OpenGL coordinates to ButtonField coordinates.
/// </summary>
/// <param name="coordinates: ">The coordinates of the click to be converted.</param>
/// <returns>Wether or not the user has clicked on the ButtonField.</returns>
bool ButtonFieldDeploy::Click(POINT& coordinates)
{
	if (coordinates.x >= this->StartX && coordinates.y >= this->StartY && coordinates.x < ButtonFieldW + this->StartX && coordinates.y < ButtonFieldH + this->StartY)
	{
		coordinates.x -= this->StartX;
		coordinates.y -= this->StartY;
		return true;
	}
	return false;
}

/// <summary>
/// Draws the ButtonField.
/// </summary>
void ButtonFieldDeploy::Draw()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.ButtonFieldDeployTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY);
	glTexCoord2d(1, 0); glVertex2f(this->StartX + 4, this->StartY);
	glTexCoord2d(1, 1); glVertex2f(this->StartX + 4, this->StartY + 3);
	glTexCoord2d(0, 1); glVertex2f(this->StartX, this->StartY + 3);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

/// <summary>
/// Deploys the current active ship.
/// </summary>
void ButtonFieldDeploy::Deploy()
{
	if (userField.Ships[engine.ShipsDeployed].Deployable) userField.Ships[engine.ShipsDeployed].Deployed = true;
	else return;
	engine.ShipsDeployed++;
	if (engine.ShipsDeployed == 10) { engine.SetMode(Engine::GAMESTATUS::MainGame); return; }
	enemyField.CloseNextShip();
	engine.MoveShipToUserField(enemyField.Ships[engine.ShipsDeployed], userField.Ships[engine.ShipsDeployed]);
}
