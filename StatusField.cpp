#include "Engine.h"
#include "StatusField.h"
#include "TextureManager.h"
#include "resource.h"

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
		if (engine.GetTurn()) BottomTextureID = textureManager.StatusFieldBottomYourTurnTextureID;
		else BottomTextureID = textureManager.StatusFieldBottomOpponentTurnTextureID;
	}
	break;
	case Engine::GAMESTATUS::ChoosingConnectionMode:
	{
		TopTextureID = textureManager.StatusFieldTopPVPTextureID;
		BottomTextureID = textureManager.StatusFieldBottomChooseConnTypeTextureID;
	}
	break;
	case Engine::GAMESTATUS::ChoosingConnectionSide:
	{
		TopTextureID = textureManager.StatusFieldTopPVPTextureID;
		BottomTextureID = textureManager.StatusFieldBottomChooseConnSideTextureID;
	}
	break;
	case Engine::GAMESTATUS::ServerConnection:
	{
		TopTextureID = textureManager.StatusFieldTopPVPTextureID;
		BottomTextureID = textureManager.StatusFieldBottomWaitingConnectionTextureID;
	}
	break;
	case Engine::GAMESTATUS::ClientConnection:
	{
		TopTextureID = textureManager.StatusFieldTopPVPTextureID;
		BottomTextureID = textureManager.StatusFieldBottomInputIPTextureID;
	}
	break;
	case Engine::GAMESTATUS::AutoConnection:
	{
		TopTextureID = textureManager.StatusFieldTopPVPTextureID;
		BottomTextureID = textureManager.StatusFieldBottomSearchingConnectionTextureID;
	}
	break;
	case Engine::GAMESTATUS::Disconnecting:
	{
		TopTextureID = textureManager.StatusFieldTopPVPTextureID;
		BottomTextureID = textureManager.StatusFieldBottomDisconnectTextureID;
	}
	break;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TopTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->startX, this->startY + StatusFieldH - 1);
	glTexCoord2d(1.f, 0); glVertex2f(this->startX + StatusFieldW, this->startY + StatusFieldH - 1);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->startX + StatusFieldW, this->startY + StatusFieldH);
	glTexCoord2d(0, 1.f); glVertex2f(this->startX,this->startY + StatusFieldH);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, BottomTextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->startX, this->startY);
	glTexCoord2d(1.f, 0); glVertex2f(this->startX + StatusFieldW, this->startY);
	glTexCoord2d(1.f, 1.f); glVertex2f(this->startX + StatusFieldW, this->startY + StatusFieldH - 1);
	glTexCoord2d(0, 1.f); glVertex2f(this->startX, this->startY + StatusFieldH - 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
