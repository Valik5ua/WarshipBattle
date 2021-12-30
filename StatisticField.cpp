#include "Engine.h"
#include "StatisticField.h"
#include "TextureManager.h"
#include "GL/glew.h"
#include "GL/glut.h"
#include "GL/freeglut.h"
#include <string>

extern Engine engine;
extern HDC hDC;
extern TextureManager textureManager;

StatisticField::StatisticField(unsigned const int StartX, unsigned const int StartY)
{
	this->StartX = StartX;
	this->StartY = StartY;
}

void StatisticField::Draw()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.StatisticFieldTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY);
	glTexCoord2d(1.f, 0); glVertex2f(StatisticFieldW + this->StartX, this->StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(StatisticFieldW + this->StartX, StatisticFieldH + this->StartY);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX, StatisticFieldH + this->StartY);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	this->DrawTime(engine.GetMatchTime());
	this->DrawKilledAlive(engine.GetPlayerShipsAlive(), engine.GetOpponentShipsAlive());
}

void StatisticField::DrawTime(unsigned int SecTime)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureManager.TimeColonTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX + 1.75, this->StartY);
	glTexCoord2d(1.f, 0); glVertex2f(this->StartX + 2.25, this->StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->StartX + 2.25, this->StartY + 1);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX + 1.75, this->StartY + 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	GLuint TextureArr[4];
	GLuint NumArr[4];
	unsigned int MinTime;
	MinTime = SecTime / 60;
	SecTime %= 60;
	NumArr[3] = SecTime % 10;
	NumArr[2] = SecTime / 10;
	NumArr[1] = MinTime % 10;
	NumArr[0] = MinTime / 10;

	for (int i = 0; i < 4; i++)
	{
		switch (NumArr[i])
		{
		case 0:
			TextureArr[i] = textureManager.TimeNumber0TextureID;
			break;
		case 1:
			TextureArr[i] = textureManager.TimeNumber1TextureID;
			break;
		case 2:
			TextureArr[i] = textureManager.TimeNumber2TextureID;
			break;
		case 3:
			TextureArr[i] = textureManager.TimeNumber3TextureID;
			break;
		case 4:
			TextureArr[i] = textureManager.TimeNumber4TextureID;
			break;
		case 5:
			TextureArr[i] = textureManager.TimeNumber5TextureID;
			break;
		case 6:
			TextureArr[i] = textureManager.TimeNumber6TextureID;
			break;
		case 7:
			TextureArr[i] = textureManager.TimeNumber7TextureID;
			break;
		case 8:
			TextureArr[i] = textureManager.TimeNumber8TextureID;
			break;
		case 9:
			TextureArr[i] = textureManager.TimeNumber9TextureID;
			break;
		}
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureArr[0]);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX + .25, this->StartY);
	glTexCoord2d(1.f, 0); glVertex2f(this->StartX + 1, this->StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->StartX + 1, this->StartY + 1);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX + .25, this->StartY + 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureArr[1]);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX + 1, this->StartY);
	glTexCoord2d(1.f, 0); glVertex2f(this->StartX + 1.75, this->StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->StartX + 1.75, this->StartY + 1);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX + 1, this->StartY + 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureArr[2]);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX + 2.25, this->StartY);
	glTexCoord2d(1.f, 0); glVertex2f(this->StartX + 3, this->StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->StartX + 3, this->StartY + 1);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX + 2.25, this->StartY + 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureArr[3]);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX + 3, this->StartY);
	glTexCoord2d(1.f, 0); glVertex2f(this->StartX + 3.75, this->StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->StartX + 3.75, this->StartY + 1);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX + 3, this->StartY + 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void StatisticField::DrawKilledAlive(unsigned int AlivePlayer, unsigned int AliveOpponent)
{
	GLuint PlayerAliveTextureID = 0;
	GLuint OpponentAliveTextureID = 0;
	GLuint PlayerDeadTextureID = 0;
	GLuint OpponentDeadTextureID = 0;

	switch (AlivePlayer)
	{
	case 0:
		PlayerAliveTextureID = textureManager.AliveDeadNumber0TextureID;
		PlayerDeadTextureID = textureManager.AliveDeadNumber10TextureID;
		break;
	case 1:
		PlayerAliveTextureID = textureManager.AliveDeadNumber1TextureID;
		PlayerDeadTextureID = textureManager.AliveDeadNumber9TextureID;
		break;
	case 2:
		PlayerAliveTextureID = textureManager.AliveDeadNumber2TextureID;
		PlayerDeadTextureID = textureManager.AliveDeadNumber8TextureID;
		break;
	case 3:
		PlayerAliveTextureID = textureManager.AliveDeadNumber3TextureID;
		PlayerDeadTextureID = textureManager.AliveDeadNumber7TextureID;
		break;
	case 4:
		PlayerAliveTextureID = textureManager.AliveDeadNumber4TextureID;
		PlayerDeadTextureID = textureManager.AliveDeadNumber6TextureID;
		break;
	case 5:
		PlayerAliveTextureID = textureManager.AliveDeadNumber5TextureID;
		PlayerDeadTextureID = textureManager.AliveDeadNumber5TextureID;
		break;
	case 6:
		PlayerAliveTextureID = textureManager.AliveDeadNumber6TextureID;
		PlayerDeadTextureID = textureManager.AliveDeadNumber4TextureID;
		break;
	case 7:
		PlayerAliveTextureID = textureManager.AliveDeadNumber7TextureID;
		PlayerDeadTextureID = textureManager.AliveDeadNumber3TextureID;
		break;
	case 8:
		PlayerAliveTextureID = textureManager.AliveDeadNumber8TextureID;
		PlayerDeadTextureID = textureManager.AliveDeadNumber2TextureID;
		break;
	case 9:
		PlayerAliveTextureID = textureManager.AliveDeadNumber9TextureID;
		PlayerDeadTextureID = textureManager.AliveDeadNumber1TextureID;
		break;
	case 10:
		PlayerAliveTextureID = textureManager.AliveDeadNumber10TextureID;
		PlayerDeadTextureID = textureManager.AliveDeadNumber0TextureID;
		break;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, PlayerAliveTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY + 5);
	glTexCoord2d(1.f, 0); glVertex2f(this->StartX + 2, this->StartY + 5);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->StartX + 2, this->StartY + 8);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX, this->StartY + 8);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, PlayerDeadTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX + 2, this->StartY + 5);
	glTexCoord2d(1.f, 0); glVertex2f(this->StartX + 4, this->StartY + 5);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->StartX + 4, this->StartY + 8);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX + 2, this->StartY + 8);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	switch (AliveOpponent)
	{
	case 0:
		OpponentAliveTextureID = textureManager.AliveDeadNumber0TextureID;
		OpponentDeadTextureID = textureManager.AliveDeadNumber10TextureID;
		break;
	case 1:
		OpponentAliveTextureID = textureManager.AliveDeadNumber1TextureID;
		OpponentDeadTextureID = textureManager.AliveDeadNumber9TextureID;
		break;
	case 2:
		OpponentAliveTextureID = textureManager.AliveDeadNumber2TextureID;
		OpponentDeadTextureID = textureManager.AliveDeadNumber8TextureID;
		break;
	case 3:
		OpponentAliveTextureID = textureManager.AliveDeadNumber3TextureID;
		OpponentDeadTextureID = textureManager.AliveDeadNumber7TextureID;
		break;
	case 4:
		OpponentAliveTextureID = textureManager.AliveDeadNumber4TextureID;
		OpponentDeadTextureID = textureManager.AliveDeadNumber6TextureID;
		break;
	case 5:
		OpponentAliveTextureID = textureManager.AliveDeadNumber5TextureID;
		OpponentDeadTextureID = textureManager.AliveDeadNumber5TextureID;
		break;
	case 6:
		OpponentAliveTextureID = textureManager.AliveDeadNumber6TextureID;
		OpponentDeadTextureID = textureManager.AliveDeadNumber4TextureID;
		break;
	case 7:
		OpponentAliveTextureID = textureManager.AliveDeadNumber7TextureID;
		OpponentDeadTextureID = textureManager.AliveDeadNumber3TextureID;
		break;
	case 8:
		OpponentAliveTextureID = textureManager.AliveDeadNumber8TextureID;
		OpponentDeadTextureID = textureManager.AliveDeadNumber2TextureID;
		break;
	case 9:
		OpponentAliveTextureID = textureManager.AliveDeadNumber9TextureID;
		OpponentDeadTextureID = textureManager.AliveDeadNumber1TextureID;
		break;
	case 10:
		OpponentAliveTextureID = textureManager.AliveDeadNumber10TextureID;
		OpponentDeadTextureID = textureManager.AliveDeadNumber0TextureID;
		break;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, OpponentAliveTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY + 1);
	glTexCoord2d(1.f, 0); glVertex2f(this->StartX + 2, this->StartY + 1);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->StartX + 2, this->StartY + 4);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX, this->StartY + 4);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, OpponentDeadTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX + 2, this->StartY + 1);
	glTexCoord2d(1.f, 0); glVertex2f(this->StartX + 4, this->StartY + 1);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->StartX + 4, this->StartY + 4);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX + 2, this->StartY + 4);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}
