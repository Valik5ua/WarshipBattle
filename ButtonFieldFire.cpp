#include "ButtonFieldFire.h"
#include "TextureManager.h"

extern TextureManager textureManager;

/// <summary>
/// The default constructor for the ButtonFieldFire class.
/// </summary>
/// <param name="StartX: ">The X position of the ButtonField.</param>
/// <param name="StartY: ">The Y position of the ButtonField.</param>
ButtonFieldFire::ButtonFieldFire(int StartX, int StartY)
{
	this->StartX = StartX;
	this->StartY = StartY;
	this->Cells[0][0].ButtonID = BF_MOVE_LEFT;
	this->Cells[0][1].ButtonID = BF_MOVE_LEFT;

	this->Cells[0][2].ButtonID = BF_MOVE_UP;

	this->Cells[1][0].ButtonID = BF_MOVE_DOWN;

	this->Cells[1][1].ButtonID = BF_RANDOMAIM;

	this->Cells[1][2].ButtonID = BF_MOVE_UP;

	this->Cells[2][0].ButtonID = BF_MOVE_DOWN;

	this->Cells[2][1].ButtonID = BF_MOVE_RIGHT;
	this->Cells[2][2].ButtonID = BF_MOVE_RIGHT;

	this->Cells[3][0].ButtonID = BF_FIRE;
	this->Cells[3][1].ButtonID = BF_FIRE;
	this->Cells[3][2].ButtonID = BF_FIRE;
}

/// <summary>
/// Changes OpenGL coordinates to ButtonField coordinates.
/// </summary>
/// <param name="coordinates: ">The coordinates of the click to be converted.</param>
/// <returns>Wether or not the user has clicked on the ButtonField.</returns>
bool ButtonFieldFire::Click(POINT& coordinates)
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
void ButtonFieldFire::Draw()
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
			if (this->Cells[i][j].ButtonID == BF_RANDOMAIM)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textureManager.Btn_RandomAimTextureID);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(1, 0); glVertex2f(i + this->StartX + 0.98f, j + this->StartY);
				glTexCoord2d(1, 1); glVertex2f(i + this->StartX + 0.98f, j + this->StartY + 0.98f);
				glTexCoord2d(0, 1); glVertex2f(i + this->StartX, j + this->StartY + 0.98f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			if (this->Cells[i][j].ButtonID == BF_FIRE && this->Cells[i][j + 1].ButtonID == BF_FIRE && this->Cells[i][j + 2].ButtonID == BF_FIRE)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textureManager.Btn_FireTextureID);

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
