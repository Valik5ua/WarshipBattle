#include "UserField.h"
#include "Engine.h"
#include "TextureManager.h"

extern TextureManager textureManager;
extern Ship* Ships[10];
extern Engine engine;

/// <summary>
/// Changes OpenGL coordinates to UserField coordinates.
/// </summary>
/// <param name="coordinates: ">The coordinates of the click to be converted.</param>
/// <returns>Wether or not the user has clicked on the UserField.</returns>
bool UserField::Click(POINT& coordinates)
{
	if (coordinates.x >= this->StartX && coordinates.y >= this->StartY && coordinates.x < UserGameFieldW + this->StartX && coordinates.y < UserGameFieldH + this->StartY)
	{
		coordinates.x -= this->StartX;
		coordinates.y -= this->StartY;
		return true;
	}
	return false;
}

/// <summary>
/// Sets the current active the status "Deployable"(true) or "Undeployable"(false)
/// </summary>
void UserField::SetShipDeployableStatus()
{
	for (int DeckCounter{}; DeckCounter < this->Ships[engine.ShipsDeployed].Size; DeckCounter++)
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
			{
				if (this->In_Range({ this->Ships[engine.ShipsDeployed].Decks[DeckCounter].Position.x + i,this->Ships[engine.ShipsDeployed].Decks[DeckCounter].Position.y + j }))
					if (this->ShipExists({ this->Ships[engine.ShipsDeployed].Decks[DeckCounter].Position.x + i,this->Ships[engine.ShipsDeployed].Decks[DeckCounter].Position.y + j }))
					{
						this->Ships[engine.ShipsDeployed].Deployable = false;
						return;
					}
			}
	this->Ships[engine.ShipsDeployed].Deployable = true;
}

/// <summary>
/// Moves the current active ship.
/// </summary>
/// <param name="Direction: ">The direction in which to move the ship.</param>
void UserField::MoveActiveShip(int Direction)
{
	switch (Direction)
	{
	case BF_MOVE_LEFT:
		if (this->Ships[engine.ShipsDeployed].Decks[0].Position.x > 0)
			for (int i{}; i < this->Ships[engine.ShipsDeployed].Size; i++)
				this->Ships[engine.ShipsDeployed].Decks[i].Position.x -= 1;
		break;
	case BF_MOVE_RIGHT:
		if (this->Ships[engine.ShipsDeployed].Decks[this->Ships[engine.ShipsDeployed].Size - 1].Position.x < UserGameFieldW - 1)
			for (int i{}; i < this->Ships[engine.ShipsDeployed].Size; i++)
				this->Ships[engine.ShipsDeployed].Decks[i].Position.x += 1;
		break;
	case BF_MOVE_UP:
		if (this->Ships[engine.ShipsDeployed].Decks[this->Ships[engine.ShipsDeployed].Size - 1].Position.y < UserGameFieldH - 1)
			for (int i{}; i < this->Ships[engine.ShipsDeployed].Size; i++)
		this->Ships[engine.ShipsDeployed].Decks[i].Position.y += 1;
		break;
	case BF_MOVE_DOWN:
		if (this->Ships[engine.ShipsDeployed].Decks[0].Position.y > 0)
			for (int i{}; i < this->Ships[engine.ShipsDeployed].Size; i++)
		this->Ships[engine.ShipsDeployed].Decks[i].Position.y -= 1;
		break;
	}
	this->SetShipMarkers();
	this->SetShipDeployableStatus();
}

/// <summary>
/// A function that checks whether or not a ship is located in specific loacation.
/// </summary>
/// <param name="Coordinates: ">The coordinates of the location to be checked.</param>
/// <returns>True if the ship is located in the loacation.</returns>
bool UserField::ShipExists(POINT Coordinates)
{
	for (int Arrnum = 0; Arrnum < engine.ShipsDeployed; Arrnum++)
		for (int DeckCounter{}; DeckCounter < this->Ships[Arrnum].Size; DeckCounter++)
			if (this->Ships[Arrnum].Decks[DeckCounter].Position.x == Coordinates.x && this->Ships[Arrnum].Decks[DeckCounter].Position.y == Coordinates.y)
			return true;
	return false;
}

/// <summary>
/// Creates all the ships to be located in the field.
/// </summary>
/// <param name="Mode: ">The mode of the creation.</param>
void UserField::SetShipMarkers()
{
	for (int i{}; i < UserGameFieldW; i++)
		for (int j{}; j < UserGameFieldH; j++)
			this->Cells[i][j].MarkedShip = false;

	for (int Shipnum{}; Shipnum < engine.ShipsDeployed; Shipnum++)
		for (int Decknum{}; Decknum < this->Ships[Shipnum].Size; Decknum++)
			this->Cells[this->Ships[Shipnum].Decks[Decknum].Position.x][this->Ships[Shipnum].Decks[Decknum].Position.y].MarkedShip = true;
}

/// <summary>
/// Draws the field.
/// </summary>
void UserField::Draw()
{
	GLuint TextureID{};
	for (int i{}; i < UserGameFieldW; i++)
	{
		for (int j{}; j < UserGameFieldH; j++)
		{
			if (!this->Cells[i][j].MarkedShip)
			{
				if (this->Cells[i][j].Missed) TextureID = textureManager.MissedTextureID;
				TextureID = textureManager.WaterTextureID;

				this->Cells[i][j].Draw({ this->StartX + i,this->StartY + j }, TextureID);
			}
		}
	}
	for (int Arrnum = 0; Arrnum < MAX_SHIPS_COUNT; Arrnum++)
		for (int DeckNum{}; DeckNum < this->Ships[Arrnum].Size; DeckNum++)
		{
			switch (this->Ships[Arrnum].Decks[DeckNum].integrityStatus)
			{
			case Deck::IntegrityStatus::Whole:
			{
				switch (this->Ships[Arrnum].Decks[DeckNum].Type)
				{
				case Deck::DeckType::Front:
					if (!this->Ships[Arrnum].Deployed && this->Ships[Arrnum].Deployable)
						TextureID = textureManager.ShipFrontDeployableTextureID;
					else if (!this->Ships[Arrnum].Deployed && !this->Ships[Arrnum].Deployable)
						TextureID = textureManager.ShipFrontUndeployableTextureID;
					else
					TextureID = textureManager.ShipFrontTextureID;
					break;
				case Deck::DeckType::Middle:
					if (!this->Ships[Arrnum].Deployed && this->Ships[Arrnum].Deployable)
						TextureID = textureManager.ShipMiddleDeployableTextureID;
					else if (!this->Ships[Arrnum].Deployed && !this->Ships[Arrnum].Deployable)
						TextureID = textureManager.ShipMiddleUndeployableTextureID;
					else
						TextureID = textureManager.ShipMiddleTextureID;
					break;
				case Deck::DeckType::Back:
					if (!this->Ships[Arrnum].Deployed && this->Ships[Arrnum].Deployable)
						TextureID = textureManager.ShipBackDeployableTextureID;
					else if (!this->Ships[Arrnum].Deployed && !this->Ships[Arrnum].Deployable)
						TextureID = textureManager.ShipBackUndeployableTextureID;
					else
						TextureID = textureManager.ShipBackTextureID;
					break;
				case Deck::DeckType::Single:
					if (!this->Ships[Arrnum].Deployed && this->Ships[Arrnum].Deployable)
						TextureID = textureManager.SingleShipDeployableTextureID;
					else if (!this->Ships[Arrnum].Deployed && !this->Ships[Arrnum].Deployable)
						TextureID = textureManager.SingleShipUndeployableTextureID;
					else
						TextureID = textureManager.SingleShipTextureID;
					break;
				};
			}
			break;
			case Deck::IntegrityStatus::Damaged:
			{
				switch (this->Ships[Arrnum].Decks[DeckNum].Type)
				{
				case Deck::DeckType::Front:
					TextureID = textureManager.ShipFrontAfireTextureID;
					break;
				case Deck::DeckType::Middle:
					TextureID = textureManager.ShipMiddleAfireTextureID;
					break;
				case Deck::DeckType::Back:
					TextureID = textureManager.ShipBackAfireTextureID;
					break;
				};
			}
			break;
			case Deck::IntegrityStatus::Killed:
			{
				switch (this->Ships[Arrnum].Decks[DeckNum].Type)
				{
				case Deck::DeckType::Front:
					TextureID = textureManager.ShipFrontCrackedTextureID;
					break;
				case Deck::DeckType::Middle:
					TextureID = textureManager.ShipMiddleCrackedTextureID;
					break;
				case Deck::DeckType::Back:
					TextureID = textureManager.ShipBackCrackedTextureID;
					break;
				case Deck::DeckType::Single:
					TextureID = textureManager.SingleShipCrackedTextureID;
					break;
				};
			}
			}
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, TextureID);

			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2f(this->Ships[Arrnum].Decks[DeckNum].Position.x + this->StartX, this->Ships[Arrnum].Decks[DeckNum].Position.y + this->StartY);
			glTexCoord2d(1.f, 0); glVertex2f(this->Ships[Arrnum].Decks[DeckNum].Position.x + 1.f + this->StartX, this->Ships[Arrnum].Decks[DeckNum].Position.y + this->StartY);
			glTexCoord2d(1.f, 1.f); glVertex2f(this->Ships[Arrnum].Decks[DeckNum].Position.x + 1.f + this->StartX, this->Ships[Arrnum].Decks[DeckNum].Position.y + 1.f + this->StartY);
			glTexCoord2d(0, 1.f); glVertex2f(this->Ships[Arrnum].Decks[DeckNum].Position.x + this->StartX, this->Ships[Arrnum].Decks[DeckNum].Position.y + 1.f + this->StartY);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
}

/// <summary>
/// Checks wether or not a certain position is in range of the field.
/// </summary>
/// <param name="Coordinates: ">The position to check.</param>
/// <returns>Wether or not the coordinates given are in range of the field.</returns>
bool UserField::In_Range(POINT Coordinates)
{
	if (Coordinates.x >= 0 && Coordinates.x < UserGameFieldW)
		if (Coordinates.y >= 0 && Coordinates.y < UserGameFieldH)
			return true;
	return false;
}
