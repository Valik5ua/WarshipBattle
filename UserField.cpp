#include "UserField.h"
#include "TextureManager.h"

extern TextureManager textureManager;

#ifdef TEXTURES_TEST

void DrawShip1(const int StartX, const int StartY, const int X, int& Y)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, SingleShipTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(X + StartX, Y + StartY);
	glTexCoord2d(1.01f, 0); glVertex2f(X + 1.f + StartX, Y + StartY);
	glTexCoord2d(1.01f, 1.01f); glVertex2f(X + 1.f + StartX, Y + 1.f + StartY);
	glTexCoord2d(0, 1.01f); glVertex2f(X + StartX, Y + 1.f + StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void DrawShip2(const int StartX, const int StartY, const int X, int& Y)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ShipBackTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(X + StartX, Y + StartY);
	glTexCoord2d(1.01f, 0); glVertex2f(X + 1.f + StartX, Y + StartY);
	glTexCoord2d(1.01f, 1.01f); glVertex2f(X + 1.f + StartX, Y + 1.f + StartY);
	glTexCoord2d(0, 1.01f); glVertex2f(X + StartX, Y + 1.f + StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ShipFrontTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(X + StartX, Y + 1.f + StartY);
	glTexCoord2d(1.01f, 0); glVertex2f(X + 1.f + StartX, Y + 1.f + StartY);
	glTexCoord2d(1.01f, 1.01f); glVertex2f(X + 1.f + StartX, Y + 2.f + StartY);
	glTexCoord2d(0, 1.01f); glVertex2f(X + StartX, Y + 2.f + StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	Y++;
}
void DrawShip3(const int StartX, const int StartY, const int X, int &Y)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ShipBackTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(X + StartX, Y + StartY);
	glTexCoord2d(1.01f, 0); glVertex2f(X + 1.f + StartX, Y + StartY);
	glTexCoord2d(1.01f, 1.01f); glVertex2f(X + 1.f + StartX, Y + 1.f + StartY);
	glTexCoord2d(0, 1.01f); glVertex2f(X + StartX, Y + 1.f + StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ShipMiddleTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(X + StartX, Y + 1.f + StartY);
	glTexCoord2d(1.01f, 0); glVertex2f(X + 1.f + StartX, Y + 1.f + StartY);
	glTexCoord2d(1.01f, 1.01f); glVertex2f(X + 1.f + StartX, Y + 2.f + StartY);
	glTexCoord2d(0, 1.01f); glVertex2f(X + StartX, Y + 2.f + StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ShipFrontTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(X + StartX, Y + 2.f + StartY);
	glTexCoord2d(1.01f, 0); glVertex2f(X + 1.f + StartX, Y + 2.f + StartY);
	glTexCoord2d(1.01f, 1.01f); glVertex2f(X + 1.f + StartX, Y + 3.f + StartY);
	glTexCoord2d(0, 1.01f); glVertex2f(X + StartX, Y + 3.f + StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	Y += 2;
}
void DrawShip4(const int StartX, const int StartY, const int X, int& Y)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ShipBackTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(X + StartX, Y + StartY);
	glTexCoord2d(1.01f, 0); glVertex2f(X + 1.f + StartX, Y + StartY);
	glTexCoord2d(1.01f, 1.01f); glVertex2f(X + 1.f + StartX, Y + 1.f + StartY);
	glTexCoord2d(0, 1.01f); glVertex2f(X + StartX, Y + 1.f + StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ShipMiddleTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(X + StartX, Y + 1.f + StartY);
	glTexCoord2d(1.f, 0); glVertex2f(X + 1.f + StartX, Y + 1.f + StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(X + 1.f + StartX, Y + 2.f + StartY);
	glTexCoord2d(0, 1.f); glVertex2f(X + StartX, Y + 2.f + StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ShipMiddleTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(X + StartX, Y + 2.f + StartY);
	glTexCoord2d(1.f, 0); glVertex2f(X + 1.f + StartX, Y + 2.f + StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(X + 1.f + StartX, Y + 3.f + StartY);
	glTexCoord2d(0, 1.f); glVertex2f(X + StartX, Y + 3.f + StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ShipFrontTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(X + StartX, Y + 3.f + StartY);
	glTexCoord2d(1.f, 0); glVertex2f(X + 1.f + StartX, Y + 3.f + StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(X + 1.f + StartX, Y + 4.f + StartY);
	glTexCoord2d(0, 1.f); glVertex2f(X + StartX, Y + 4.f + StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	Y += 3;
}

#endif //TEXTURES_TEST

/// <summary>
/// Changes OpenGL coordinates to field coordinates.
/// </summary>
/// <param name="coordinates"></param>
/// <returns></returns>
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

/// <summary>
/// Draws the Field
/// </summary>
void UserField::Draw()
{
	for (int i{}; i < MyGameFieldW; i++)
	{
		for (int j{}; j < MyGameFieldH; j++)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureManager.WaterTextureID);

			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2f(i+this->StartX, j + this->StartY);
			glTexCoord2d(1.f, 0); glVertex2f(i + 1.f + this->StartX, j + this->StartY);
			glTexCoord2d(1.f, 1.f); glVertex2f(i+ 1.f + this->StartX, j + 1.f + this->StartY);
			glTexCoord2d(0, 1.f); glVertex2f(i + this->StartX, j + 1.f + this->StartY);
			glEnd();

			glDisable(GL_TEXTURE_2D);
		}
	}
}

void UserField::Select(const size_t CellX, const size_t CellY)
{
	Deselect();
	this->Cells[CellX][CellY].Selected = true;
}

void UserField::Deselect()
{
	for (int i{}; i<MyGameFieldW;i++)
	{
		for (int j{}; j < MyGameFieldH; j++)
		{
			if (this->Cells[i][j].Selected) { this->Cells[i][j].Selected = false; return; }
		}
	}
}
