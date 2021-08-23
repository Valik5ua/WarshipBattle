#include "UserField.h"
#include "TextureManager.h"

extern TextureManager textureManager;

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

/// <summary>
/// Draws the Field.
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
