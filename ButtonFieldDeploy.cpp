#include "ButtonFieldDeploy.h"
#include "TextureManager.h"
#include "UserField.h"
#include "EnemyField.h"
#include "Engine.h"

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
	for (int i = 0; i < ButtonFieldW; i++)
		for (int j = 0; j < ButtonFieldH; j++)
		{
			if (this->Cells[i][j].ButtonID == BF_MOVE_UP && this->Cells[i + 1][j].ButtonID == BF_MOVE_UP)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textureManager.Btn_UpTextureID);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(1, 0); glVertex2f(i + this->StartX + 1.98f, j + this->StartY);
				glTexCoord2d(1, 1); glVertex2f(i + this->StartX + 1.98f, j + this->StartY + 0.98f);
				glTexCoord2d(0, 1); glVertex2f(i + this->StartX, j + this->StartY + 0.98f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			if (this->Cells[i][j].ButtonID == BF_MOVE_DOWN && this->Cells[i + 1][j].ButtonID == BF_MOVE_DOWN)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textureManager.Btn_DownTextureID);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(1, 0); glVertex2f(i + this->StartX + 1.98f, j + this->StartY);
				glTexCoord2d(1, 1); glVertex2f(i + this->StartX + 1.98f, j + this->StartY + 0.98f);
				glTexCoord2d(0, 1); glVertex2f(i + this->StartX, j + this->StartY + 0.98f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			if (this->Cells[i][j].ButtonID == BF_MOVE_LEFT && this->Cells[i][j + 1].ButtonID == BF_MOVE_LEFT)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textureManager.Btn_LeftTextureID);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(1, 0); glVertex2f(i + this->StartX + 0.98f, j + this->StartY);
				glTexCoord2d(1, 1); glVertex2f(i + this->StartX + 0.98f, j + this->StartY + 1.98f);
				glTexCoord2d(0, 1); glVertex2f(i + this->StartX, j + this->StartY + 1.98f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			if (this->Cells[i][j].ButtonID == BF_MOVE_RIGHT && this->Cells[i][j + 1].ButtonID == BF_MOVE_RIGHT)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textureManager.Btn_RightTextureID);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(1, 0); glVertex2f(i + this->StartX + 0.98f, j + this->StartY);
				glTexCoord2d(1, 1); glVertex2f(i + this->StartX + 0.98f, j + this->StartY + 1.98f);
				glTexCoord2d(0, 1); glVertex2f(i + this->StartX, j + this->StartY + 1.98f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			if (this->Cells[i][j].ButtonID == BF_ROTATE)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textureManager.Btn_RotateTextureID);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(1, 0); glVertex2f(i + this->StartX + 0.98f, j + this->StartY);
				glTexCoord2d(1, 1); glVertex2f(i + this->StartX + 0.98f, j + this->StartY + 0.98f);
				glTexCoord2d(0, 1); glVertex2f(i + this->StartX, j + this->StartY + 0.98f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			if (this->Cells[i][j].ButtonID == BF_DEPLOY && this->Cells[i][j + 1].ButtonID == BF_DEPLOY && this->Cells[i][j + 2].ButtonID == BF_DEPLOY)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textureManager.Btn_DeployTextureID);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(1, 0); glVertex2f(i + this->StartX + 0.98f, j + this->StartY);
				glTexCoord2d(1, 1); glVertex2f(i + this->StartX + 0.98f, j + this->StartY + 2.98f);
				glTexCoord2d(0, 1); glVertex2f(i + this->StartX, j + this->StartY + 2.98f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
				return;
			}
		}
}

void ButtonFieldDeploy::Deploy()
{
	if (userField.Ships[engine.ShipsDeployed].Deployable) userField.Ships[engine.ShipsDeployed].Deployed = true;
	else return;
	engine.ShipsDeployed++;
	if (engine.ShipsDeployed == 10) { engine.SetMode(Engine::MODE::MainGame); return; }
	enemyField.CloseNextShip();
	engine.MoveShipToUserField(enemyField.Ships[engine.ShipsDeployed], userField.Ships[engine.ShipsDeployed]);
}
