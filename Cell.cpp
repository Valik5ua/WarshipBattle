#include "Cell.h"

/// <summary>
/// The default constructor for the cell class.
/// </summary>
Cell::Cell() :ButtonID(0), Open(true), Cell_Aim(false)
{
}

void Cell::Draw(POINT Coordinates, GLuint TextureID)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(Coordinates.x, Coordinates.y);
	glTexCoord2d(1.f, 0); glVertex2f(1.f + Coordinates.x, Coordinates.y);
	glTexCoord2d(1.f, 1.f); glVertex2f(1.f + Coordinates.x, 1.f + Coordinates.y);
	glTexCoord2d(0, 1.f); glVertex2f(Coordinates.x, 1.f + Coordinates.y);
	glEnd();
}
