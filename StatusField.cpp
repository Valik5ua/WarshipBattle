#include "StatusField.h"
#include "TextureManager.h"
#include "resource.h"
#include "Engine.h"

extern TextureManager textureManager;
extern Engine engine;

void StatusField::Draw()
{
	GLuint TopTextureID = 0;
	GLuint BottomTextureID = 0;

	switch (engine.GameStatus)
	{
	case Engine::GAMESTATUS::NewGame:
	{
		TopTextureID = textureManager.StatusFieldTopMainMenuTextureID;
		BottomTextureID = textureManager.StatusFieldBottomChooseGameTextureID;
	}
	break;
	case Engine::GAMESTATUS::Deploying:
	{
		TopTextureID = textureManager.StatusFieldTopPVETextureID;
		BottomTextureID = textureManager.StatusFieldBottomDeployShipsTextureID;
	}
	break;
	case Engine::GAMESTATUS::MainGame:
	{
		TopTextureID = textureManager.StatusFieldTopPVETextureID;
		if (engine.GetTurn())
			BottomTextureID = textureManager.StatusFieldBottomYourTurnTextureID;
		else BottomTextureID = textureManager.StatusFieldBottomOpponentTurnTextureID;
	}
	break;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TopTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY + StatusFieldH - 1);
	glTexCoord2d(1.f, 0); glVertex2f(this->StartX + StatusFieldW, this->StartY + StatusFieldH - 1);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->StartX + StatusFieldW, this->StartY + StatusFieldH);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX,this->StartY + StatusFieldH);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, BottomTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->StartX, this->StartY);
	glTexCoord2d(1.f, 0); glVertex2f(this->StartX + StatusFieldW, this->StartY);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->StartX + StatusFieldW, this->StartY + StatusFieldH-1);
	glTexCoord2d(0, 1.f); glVertex2f(this->StartX, this->StartY + StatusFieldH-1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
