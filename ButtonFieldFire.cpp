#include "ButtonFieldFire.h"
#include "TextureManager.h"
#include "resource.h"

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
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.ButtonFieldFireTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY);
	glTexCoord2d(1, 0); glVertex2f(this->StartX + 4, this->StartY);
	glTexCoord2d(1, 1); glVertex2f(this->StartX + 4, this->StartY + 3);
	glTexCoord2d(0, 1); glVertex2f(this->StartX, this->StartY + 3);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
