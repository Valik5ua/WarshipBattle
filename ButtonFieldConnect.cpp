#include "ButtonFieldConnect.h"
#include "TextureManager.h"

extern TextureManager textureManager;

/// <summary>
/// Default constructor for the ButtonFieldConnect class.
/// </summary>
/// <param name="StartX">The X coordinate of the ButtonField.</param>
/// <param name="StartY">The Y coordinate of the ButtonField.</param>
ButtonFieldConnect::ButtonFieldConnect(int StartX, int StartY)
{
	this->StartX = StartX;
	this->StartY = StartY;
	this->Cells[0][0].ButtonID = BF_CANCEL;
	this->Cells[0][1].ButtonID = BF_DISCONNECT;

	this->Cells[0][2].ButtonID = BF_CONNECT;

	this->Cells[1][0].ButtonID = BF_CANCEL;

	this->Cells[1][1].ButtonID = BF_DISCONNECT;

	this->Cells[1][2].ButtonID = BF_CONNECT;

	this->Cells[2][0].ButtonID = BF_CANCEL;

	this->Cells[2][1].ButtonID = BF_DISCONNECT;
	this->Cells[2][2].ButtonID = BF_CONNECT;

}

/// <summary>
/// Changes OpenGL coordinates to ButtonField coordinates.
/// </summary>
/// <param name="coordinates: ">The coordinates of the click to be converted.</param>
/// <returns>Wether or not the user has clicked on the ButtonField.</returns>
bool ButtonFieldConnect::Click(POINT& coordinates)
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
void ButtonFieldConnect::Draw()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.Btn_ConnectTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, ButtonFieldH + this->StartY - 1);
	glTexCoord2d(1, 0); glVertex2f(this->StartX + 2.98f, ButtonFieldH + this->StartY - 1);
	glTexCoord2d(1, 1); glVertex2f(this->StartX + 2.98f, ButtonFieldH + this->StartY);
	glTexCoord2d(0, 1); glVertex2f(this->StartX, ButtonFieldH + this->StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.Btn_DisconnectTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, ButtonFieldH + this->StartY - 2);
	glTexCoord2d(1, 0); glVertex2f(this->StartX + 2.98f, ButtonFieldH + this->StartY - 2);
	glTexCoord2d(1, 1); glVertex2f(this->StartX + 2.98f, ButtonFieldH + this->StartY - 1);
	glTexCoord2d(0, 1); glVertex2f(this->StartX, ButtonFieldH + this->StartY - 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.Btn_CancelTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY);
	glTexCoord2d(1, 0); glVertex2f(this->StartX + 2.98f, this->StartY);
	glTexCoord2d(1, 1); glVertex2f(this->StartX + 2.98f, this->StartY + 1);
	glTexCoord2d(0, 1); glVertex2f(this->StartX, this->StartY + 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
