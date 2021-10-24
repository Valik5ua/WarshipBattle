#include "StatusField.h"
#include "resource.h"
#include "TextureManager.h"

extern TextureManager textureManager;

void StatusField::Draw()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.StatusFieldTopPVETextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY + StatusFieldH - 1);
	glTexCoord2d(1.f, 0); glVertex2f(this->StartX + StatusFieldW, this->StartY + StatusFieldH - 1);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->StartX + StatusFieldW, this->StartY + StatusFieldH);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX,this->StartY + StatusFieldH);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.StatusFieldBottomYourTurnTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY);
	glTexCoord2d(1.f, 0); glVertex2f(this->StartX + StatusFieldW, this->StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->StartX + StatusFieldW, this->StartY + StatusFieldH-1);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX, this->StartY + StatusFieldH-1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
