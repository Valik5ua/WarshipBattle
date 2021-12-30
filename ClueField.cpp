#include "Engine.h"
#include "ClueField.h"
#include "resource.h"
#include "TextureManager.h"

extern TextureManager textureManager;
extern Engine engine;

ClueField::ClueField(const unsigned int StartX, const unsigned int StartY) :startX(StartX), startY(StartY)
{
	searchingIcon.StartX = this->startX + 5;
	searchingIcon.MaxPosX = searchingIcon.StartX + 4;
	searchingIcon.Direction = true;

	disconnectingDots.StartX = this->startX + 10.75f;
	disconnectingDots.NumOfDots = 1;
	disconnectingDots.FrameCounter = 0;
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
	case Engine::GAMESTATUS::ChoosingConnectionMode:
	{
		TextureID = textureManager.ClueFieldChooseConnTypeTextureID;
	}
	break;
	case Engine::GAMESTATUS::ChoosingConnectionSide:
	{
		TextureID = textureManager.ClueFieldChooseConnSideTextureID;
	}
	break;
	case Engine::GAMESTATUS::ServerConnection:
	{
		TextureID = textureManager.ClueFieldShowIPTextureID;
	}
	break;
	case Engine::GAMESTATUS::ClientConnection:
	{
		TextureID = textureManager.ClueFieldInputIPTextureID;
	}
	break;
	case Engine::GAMESTATUS::AutoConnection:
	{
		TextureID = textureManager.ClueFieldConnSearchTextureID;
		if (searchingIcon.Direction)
		{
			searchingIcon.StartX += (float)(4 / (float)40);
			if (searchingIcon.StartX >= searchingIcon.MaxPosX)
				searchingIcon.Direction = !searchingIcon.Direction;
		}
		else
		{
			searchingIcon.StartX -= (float)(4 / (float)40);
			if (searchingIcon.StartX <= searchingIcon.MaxPosX - 4)
				searchingIcon.Direction = !searchingIcon.Direction;
		}
	}
	break;
	case Engine::GAMESTATUS::Disconnecting:
	{
		TextureID = textureManager.ClueFieldDisconnectingTextureID;
		if (++disconnectingDots.FrameCounter == disconnectingDots.MaxFrames) disconnectingDots.FrameCounter = 0;
		disconnectingDots.NumOfDots = disconnectingDots.FrameCounter / (this->disconnectingDots.MaxFrames / 3);
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
	glDisable(GL_TEXTURE_2D);

	if (engine.GameStatus == Engine::GAMESTATUS::AutoConnection)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureManager.SearchingIconTextureID);

		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex2f(this->searchingIcon.StartX, this->startY + 0.25f);
		glTexCoord2d(1.f, 0); glVertex2f(1.5f + this->searchingIcon.StartX, this->startY + 0.25f);
		glTexCoord2d(1.f, 1.f); glVertex2f(1.5f + this->searchingIcon.StartX, 1.75f + this->startY);
		glTexCoord2d(0, 1.f); glVertex2f(this->searchingIcon.StartX, 1.75f + this->startY);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	if (engine.GameStatus == Engine::GAMESTATUS::Disconnecting)
	{
		for (int i = 0; i <= disconnectingDots.NumOfDots; i++)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureManager.DisconnectingDotsTextureID);

			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2f(this->disconnectingDots.StartX + (i / (float)3), this->startY + 0.65f);
			glTexCoord2d(1.f, 0); glVertex2f(this->disconnectingDots.StartX+ 0.16f+(i/(float)3), this->startY + 0.65f);
			glTexCoord2d(1.f, 1.f); glVertex2f(this->disconnectingDots.StartX +0.16f+ (i / (float)3), 0.81f + this->startY);
			glTexCoord2d(0, 1.f); glVertex2f(this->disconnectingDots.StartX + (i / (float)3), 0.81f + this->startY);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	}
}
