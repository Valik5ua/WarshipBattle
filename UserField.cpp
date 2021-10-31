#include "UserField.h"
#include "Engine.h"
#include "TextureManager.h"
#include "resource.h"
#include <thread>

extern TextureManager textureManager;
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
				if (this->In_Range({ this->Ships[engine.ShipsDeployed].Decks[DeckCounter].Position.x + i, this->Ships[engine.ShipsDeployed].Decks[DeckCounter].Position.y + j }))
					if (this->ShipExists({ this->Ships[engine.ShipsDeployed].Decks[DeckCounter].Position.x + i, this->Ships[engine.ShipsDeployed].Decks[DeckCounter].Position.y + j }, engine.ShipsDeployed) >= 0)
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
		if (this->In_Range({ this->Ships[engine.ShipsDeployed].Decks[0].Position.x - 1,this->Ships[engine.ShipsDeployed].Decks[0].Position.y }))
			for (int i{}; i < this->Ships[engine.ShipsDeployed].Size; i++)
				this->Ships[engine.ShipsDeployed].Decks[i].Position.x -= 1;
		break;
	case BF_MOVE_RIGHT:
		if (this->In_Range({ this->Ships[engine.ShipsDeployed].Decks[this->Ships[engine.ShipsDeployed].Size - 1].Position.x + 1,this->Ships[engine.ShipsDeployed].Decks[0].Position.y }))
			for (int i{}; i < this->Ships[engine.ShipsDeployed].Size; i++)
				this->Ships[engine.ShipsDeployed].Decks[i].Position.x += 1;
		break;
	case BF_MOVE_UP:
		if (this->In_Range({ this->Ships[engine.ShipsDeployed].Decks[0].Position.x,this->Ships[engine.ShipsDeployed].Decks[this->Ships[engine.ShipsDeployed].Size-1].Position.y + 1 }))
			for (int i{}; i < this->Ships[engine.ShipsDeployed].Size; i++)
				this->Ships[engine.ShipsDeployed].Decks[i].Position.y += 1;
		break;
	case BF_MOVE_DOWN:
		if (this->In_Range({ this->Ships[engine.ShipsDeployed].Decks[0].Position.x,this->Ships[engine.ShipsDeployed].Decks[0].Position.y - 1 }))
			for (int i{}; i < this->Ships[engine.ShipsDeployed].Size; i++)
				this->Ships[engine.ShipsDeployed].Decks[i].Position.y -= 1;
		break;
	}
	this->SetShipMarkers();
	this->SetShipDeployableStatus();
}

void UserField::RotateActiveShip()
{
	Ship TempShip(this->Ships[engine.ShipsDeployed]);
	TempShip.Rotated = !TempShip.Rotated;

	switch (this->Ships[engine.ShipsDeployed].Rotated)
	{
	case true:
	{
		for (int i = 0; i < TempShip.Size; i++)
		{
			TempShip.Decks[i].Position.y += i;
			TempShip.Decks[i].Position.x -= i;
		}
		if (!this->In_Range(TempShip.Decks[TempShip.Size - 1].Position))
		{
			do
			{
				for (int Decknum = 0; Decknum < TempShip.Size; Decknum++)
				{
					TempShip.Decks[Decknum].Position.y -= 1;
				}
				if (this->In_Range(TempShip.Decks[TempShip.Size - 1].Position))
					break;
			} while (true);
			break;
		}
	}
	break;
	case false:
	{
		for (int i = 0; i < TempShip.Size; i++)
		{
			TempShip.Decks[i].Position.y -= i;
			TempShip.Decks[i].Position.x += i;
		}
		if (!this->In_Range(TempShip.Decks[TempShip.Size-1].Position))
		{
			do
			{
				for (int Decknum = 0; Decknum < TempShip.Size; Decknum++)
				{
					TempShip.Decks[Decknum].Position.x -= 1;
				}
				if (this->In_Range(TempShip.Decks[TempShip.Size - 1].Position))
					break;
			} while (true);
			break;
		}
	}
	}
	this->Ships[engine.ShipsDeployed] = TempShip;
	this->SetShipMarkers();
	this->SetShipDeployableStatus();
}

/// <summary>
/// A function that checks whether or not a ship is located in specific loacation.
/// </summary>
/// <param name="Coordinates: ">The coordinates of the location to be checked.</param>
/// <returns>True if the ship is located in the loacation.</returns>
int UserField::ShipExists(POINT Coordinates, unsigned const int ShipsToCheck)
{
	for (int Arrnum = 0; Arrnum < ShipsToCheck; Arrnum++)
		for (int DeckCounter{}; DeckCounter < this->Ships[Arrnum].Size; DeckCounter++)
			if (this->Ships[Arrnum].Decks[DeckCounter].Position.x == Coordinates.x && this->Ships[Arrnum].Decks[DeckCounter].Position.y == Coordinates.y)
			return Arrnum;
	return -1;
}

void UserField::SwapActiveShip(const unsigned int ShipNum)
{
	Ship tempShip(this->Ships[ShipNum]);
	this->Ships[ShipNum] = this->Ships[engine.ShipsDeployed];
	this->Ships[engine.ShipsDeployed] = tempShip;
	this->Ships[ShipNum].Deployed = true;
	this->Ships[engine.ShipsDeployed].Deployed = false;
	this->Ships[engine.ShipsDeployed].Deployable = true;
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
				else TextureID = textureManager.WaterTextureID;

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
			this->Cells[this->Ships[Arrnum].Decks[DeckNum].Position.x][this->Ships[Arrnum].Decks[DeckNum].Position.y]
				.Draw({ this->Ships[Arrnum].Decks[DeckNum].Position.x + this->StartX,this->Ships[Arrnum].Decks[DeckNum].Position.y + this->StartY }
			, TextureID);

			if (this->Ships[Arrnum].Rotated)
			{
				this->Cells[this->Ships[Arrnum].Decks[DeckNum].Position.x][this->Ships[Arrnum].Decks[DeckNum].Position.y]
					.Draw({ this->Ships[Arrnum].Decks[DeckNum].Position.x + this->StartX,this->Ships[Arrnum].Decks[DeckNum].Position.y + this->StartY }
				, TextureID, true);
			}

		}
}

/// <summary>
/// Clears the field of any markers set before.
/// </summary>
void UserField::ClearField()
{
	for (int i{}; i < 10; i++)
		for (int j{}; j < 10; j++)
		{
			this->Cells[i][j].Open = true;
			this->Cells[i][j].MarkedShip = false;
			this->Cells[i][j].Missed = false;
			this->Cells[i][j].Cell_Aim = false;
		}
}

/// <summary>
/// Cleans all the ships of any markers set before.
/// </summary>
void UserField::CleanShips()
{
	for (int i{}; i < MAX_SHIPS_COUNT; i++)
		this->Ships[i].Size = 0;
}

void UserField::SetAimPoint(POINT AimPoint)
{
	this->AimPoint = AimPoint;
}

int UserField::ShootRecieve(const POINT ShootCoordinates)
{
	std::thread th(&UserField::ThreadFunc, this, ShootCoordinates);
	th.detach();

	int ShipID = this->ShipExists(ShootCoordinates, MAX_SHIPS_COUNT);
	if (ShipID >= 0)
	{
		Ship TempShip = this->Ships[ShipID];
		TempShip.SetDamageToDeck(ShootCoordinates);

		if (TempShip.Killed)
			return TempShip.Size;
		else
			return Engine::ShootStatus::Damage;
	}
	else
	{
		return Engine::ShootStatus::Miss;
	}
}

POINT UserField::ShootCreate()
{
	return this->AimPoint;
}

void UserField::ShootAnswer(const int status)
{
}

void UserField::ThreadFunc(const POINT ShootCoordinates)
{
	while (engine.Animation)
	{
	}
	int ShipID = this->ShipExists(ShootCoordinates, MAX_SHIPS_COUNT);
	if (ShipID >= 0)
	{
		this->Ships[ShipID].SetDamageToDeck(ShootCoordinates);
	}
	else
	{
		engine.SwitchTurns();
		this->Cells[ShootCoordinates.x][ShootCoordinates.y].Missed = true;
	}
	engine.LastShotAccomplished = true;
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
