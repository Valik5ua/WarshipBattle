#include "Engine.h"
#include "ButtonFieldConnect.h"
#include "TextureManager.h"
#include "resource.h"

extern TextureManager textureManager;
extern Engine engine;

/// <summary>
/// Default constructor for the ButtonFieldConnect class.
/// </summary>
/// <param name="StartX">The X coordinate of the ButtonField.</param>
/// <param name="StartY">The Y coordinate of the ButtonField.</param>
ButtonFieldConnect::ButtonFieldConnect(int StartX, int StartY)
{
	this->StartX = StartX;
	this->StartY = StartY;
	for (int i = 0; i < 4; i++)
	{
		this->Cells[i][0].ButtonID = BF_CONNECT_BOTTOM_BUTTON;
		this->Cells[i][1].ButtonID = BF_CONNECT_MIDDLE_BUTTON;
		this->Cells[i][2].ButtonID = BF_CONNECT_TOP_BUTTON;
	}
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
	GLuint TextureID{};
	switch (engine.GameStatus)
	{
	case Engine::GAMESTATUS::ChoosingConnectionMode:
	{
		TextureID = textureManager.ButtonFieldConnect_AutoManualTextureID;
	}
	break;
	case Engine::GAMESTATUS::ChoosingConnectionSide:
	{
		TextureID = textureManager.ButtonFieldConnect_M_ServerClientTextureID;
	}
	break;
	case Engine::GAMESTATUS::ServerConnection:
	{
		TextureID = textureManager.ButtonFieldConnect_M_ServerConnectionTextureID;
	}
	break;
	case Engine::GAMESTATUS::ClientConnection:
	{
		TextureID = textureManager.ButtonFieldConnect_M_ClientConnectionTextureID;
	}
	break;
	case Engine::GAMESTATUS::AutoConnection:
	{
		TextureID = textureManager.ButtonFieldConnect_A_ConnectionTextureID;
	}
	break;
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY);
	glTexCoord2d(1, 0); glVertex2f(this->StartX + ButtonFieldW, this->StartY);
	glTexCoord2d(1, 1); glVertex2f(this->StartX + ButtonFieldW, ButtonFieldH + this->StartY);
	glTexCoord2d(0, 1); glVertex2f(this->StartX, ButtonFieldH + this->StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
