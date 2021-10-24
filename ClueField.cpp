#include "ClueField.h"
#include "resource.h"
#include "TextureManager.h"

extern TextureManager textureManager;

ClueField::ClueField(const unsigned int StartX, const unsigned int StartY)
{
	this->StartX = StartX;
	this->StartY = StartY;
}

void ClueField::Draw()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.ClueFieldTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY);
	glTexCoord2d(1.f, 0); glVertex2f(ClueFieldW + this->StartX, this->StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(ClueFieldW + this->StartX, ClueFieldH + this->StartY);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX, ClueFieldH + this->StartY);
	glEnd();
}
