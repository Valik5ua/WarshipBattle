#include "ButtonFieldNewGame.h"
#include "TextureManager.h"

extern TextureManager textureManager;

ButtonFieldNewGame::ButtonFieldNewGame(const int StartX, const int StartY)
{
	this->StartX = StartX;
	this->StartY = StartY;
	this->Cells[0][0].ButtonID = BF_NEWGAME_PVE;
	this->Cells[0][1].ButtonID = BF_NEWGAME_PVP;
	this->Cells[0][2].ButtonID = MSG_VOID;

	this->Cells[1][0].ButtonID = BF_NEWGAME_PVE;
	this->Cells[1][1].ButtonID = BF_NEWGAME_PVP;
	this->Cells[1][2].ButtonID = MSG_VOID;

	this->Cells[2][0].ButtonID = BF_NEWGAME_PVE;
	this->Cells[2][1].ButtonID = BF_NEWGAME_PVP;
	this->Cells[2][2].ButtonID = MSG_VOID;

	this->Cells[3][0].ButtonID = BF_NEWGAME_PVE;
	this->Cells[3][1].ButtonID = BF_NEWGAME_PVP;
	this->Cells[3][2].ButtonID = MSG_VOID;
}

bool ButtonFieldNewGame::Click(POINT& coordinates)
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
void ButtonFieldNewGame::Draw()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.Label_NewGame);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, ButtonFieldH + this->StartY - 1);
	glTexCoord2d(1, 0); glVertex2f(this->StartX + 3.98f, ButtonFieldH + this->StartY - 1);
	glTexCoord2d(1, 1); glVertex2f(this->StartX + 3.98f, ButtonFieldH + this->StartY);
	glTexCoord2d(0, 1); glVertex2f(this->StartX, ButtonFieldH + this->StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.Btn_NewGame_PVP);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, ButtonFieldH + this->StartY - 2);
	glTexCoord2d(1, 0); glVertex2f(this->StartX + 3.98f, ButtonFieldH + this->StartY - 2);
	glTexCoord2d(1, 1); glVertex2f(this->StartX + 3.98f, ButtonFieldH + this->StartY-1);
	glTexCoord2d(0, 1); glVertex2f(this->StartX, ButtonFieldH + this->StartY-1);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.Btn_NewGame_PVE);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY);
	glTexCoord2d(1, 0); glVertex2f(this->StartX + 3.98f, this->StartY);
	glTexCoord2d(1, 1); glVertex2f(this->StartX + 3.98f, this->StartY + 1);
	glTexCoord2d(0, 1); glVertex2f(this->StartX, this->StartY + 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}