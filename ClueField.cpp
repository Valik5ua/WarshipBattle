#include "ClueField.h"
#include "Engine.h"
#include "resource.h"
#include "TextureManager.h"

extern TextureManager textureManager;
extern Engine engine;

ClueField::ClueField(const unsigned int StartX, const unsigned int StartY) :startX(StartX), startY(StartY)
{
}

void ClueField::Draw()
{
	GLuint TextureID = 0;

	switch (engine.GameStatus)
	{
	case Engine::GAMESTATUS::NewGame:
	{
		switch (engine.lastGameResults)
		{
		case Engine::LastGameResults::N_A:
		{
			TextureID = textureManager.ClueFieldNewGameTextureID;
		}
		break;
		case Engine::LastGameResults::UserWon:
		{
			TextureID = textureManager.ClueFieldYouWonTextureID;
		}
		break;
		case Engine::LastGameResults::OpponentWon:
		{
			TextureID = textureManager.ClueFieldYouLostTextureID;
		}
		break;
		}
	}
	break;
	case Engine::GAMESTATUS::Deploying:
	{
		TextureID = textureManager.ClueFieldDeployingTextureID;
	}
	break;
	case Engine::GAMESTATUS::MainGame:
	{
		TextureID = textureManager.ClueFieldMainGameTextureID;
	}
	break;
	case Engine::GAMESTATUS::Connecting:
	{
		switch (engine.ConnectionStatus)
		{
		case Engine::CONNECTIONSTATUS::ChoosingConnectionType:
		{
			TextureID = textureManager.ClueFieldChooseConnTypeTextureID;
		}
		break;
		case Engine::CONNECTIONSTATUS::ChoosingConnectionSide:
		{
			TextureID = textureManager.ClueFieldChooseConnSideTextureID;
		}
		break;
		case Engine::CONNECTIONSTATUS::ServerConnection:
		{
			TextureID = textureManager.ClueFieldChooseConnTypeTextureID;
		}
		break;
		case Engine::CONNECTIONSTATUS::ClientConnection:
		{
			TextureID = textureManager.ClueFieldChooseConnTypeTextureID;
		}
		break;
		case Engine::CONNECTIONSTATUS::AutoConnection:
		{
			TextureID = textureManager.ClueFieldChooseConnTypeTextureID;
		}
		break;
		}
	}
	break;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(this->startX, this->startY);
	glTexCoord2d(1.f, 0); glVertex2f(ClueFieldW + this->startX, this->startY);
	glTexCoord2d(1.f, 1.f); glVertex2f(ClueFieldW + this->startX, ClueFieldH + this->startY);
	glTexCoord2d(0, 1.f); glVertex2f(this->startX, ClueFieldH + this->startY);
	glEnd();
}
