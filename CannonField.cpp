#include "CannonField.h"
#include "TextureManager.h"
#include "resource.h"
#include <math.h>

extern TextureManager textureManager;

void CannonField::Draw()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.CannonBackroundTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY);
	glTexCoord2d(1.f, 0); glVertex2f(CannonFieldW + this->StartX, this->StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(CannonFieldW + this->StartX, CannonFieldH + this->StartY);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX, CannonFieldH + this->StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	const unsigned int num_segments = 360;
	float theta = 0;
	float angleincrease = 1;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.CannonTextureID);
	
	glBegin(GL_TRIANGLE_FAN);
	for (int i = theta; i < num_segments; i++)
	{
		float x = (float)116/ (float)128 * cosf(theta); //calculate current x in the segment
		float y = (float)116/ (float)128 * sinf(theta); //calculate current y in the segment

		glTexCoord2d(.5 + cosf(theta + this->angle) / 2, .5 + sinf(theta + this->angle) / 2); glVertex2f(x + this->StartX + 1, y + this->StartY + 1);
		
		theta += angleincrease;
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void CannonField::Draw(float angle)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.CannonBackroundTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY);
	glTexCoord2d(1.f, 0); glVertex2f(CannonFieldW + this->StartX, this->StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(CannonFieldW + this->StartX, CannonFieldH + this->StartY);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX, CannonFieldH + this->StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	const unsigned int num_segments = 360;
	float theta = 0;
	float angleincrease = 1;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.CannonTextureID);

	glBegin(GL_TRIANGLE_FAN);
	for (int i = theta; i < num_segments; i++)
	{
		float x = (float)116 / (float)128 * cosf(theta); //calculate current x in the segment
		float y = (float)116 / (float)128 * sinf(theta); //calculate current y in the segment

		glTexCoord2d(.5 + cosf(theta + angle) / 2, .5 + sinf(theta + angle) / 2); glVertex2f(x + this->StartX + 1, y + this->StartY + 1);

		theta += angleincrease;
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
