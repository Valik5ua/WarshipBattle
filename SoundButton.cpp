#include "SoundButton.h"
#include "TextureManager.h"
#include "resource.h"

extern TextureManager textureManager;

SoundButton::SoundButton(const unsigned int StartX, const unsigned int StartY)
:Field(StartX,StartY), State(STATE::On)
{
}

bool SoundButton::Click(POINT& coordinates)
{
	if (coordinates.x >= this->StartX && coordinates.y >= this->StartY && coordinates.x < SoundButtonW + this->StartX && coordinates.y < SoundButtonH + this->StartY)
	{
		coordinates.x -= this->StartX;
		coordinates.y -= this->StartY;
		return true;
	}
	return false;
}

void SoundButton::Draw()
{
	GLuint TextureID{};
	switch (this->State)
	{
	case STATE::On:
	{
		TextureID = textureManager.SoundOnTextureID;
	}
	break;
	case STATE::Off:
	{
		TextureID = textureManager.SoundOffTextureID;
	}
	break;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY);
	glTexCoord2d(1.f, 0); glVertex2f(4.f + this->StartX, this->StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(4.f + this->StartX, 1.f + this->StartY);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX, 1.f + this->StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
