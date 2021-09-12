#include "EnemyField.h"
#include "UserField.h"
#include "TextureManager.h"

extern TextureManager textureManager;
extern UserField userField;
extern Engine engine;

/// <summary>
/// Changes OpenGL coordinates to EnemyField coordinates.
/// </summary>
/// <param name="coordinates: ">The coordinates of the click to be converted.</param>
/// <returns>Wether or not the user has clicked on the Enemyfield.</returns>
bool EnemyField::Click(POINT& coordinates)
{
	if (coordinates.x >= this->StartX && coordinates.y >= this->StartY && coordinates.x < OpponentGameFieldW + this->StartX && coordinates.y < OpponentGameFieldH + this->StartY)
	{
		coordinates.x -= this->StartX;
		coordinates.y -= this->StartY;
		return true;
	}
	return false;
}


/// <summary>
/// Moves the selected cell of the EnemyField.
/// </summary>
/// <param name="Direction: ">The direction of the arrow of where to move the selection.</param>
/// <returns>Wether or not the direction was valid.</returns>
bool EnemyField::MoveSelection(int Direction)
{
	for (int x{}; x < OpponentGameFieldW; x++)
	{
		for (int y{}; y < OpponentGameFieldH; y++)
		{
			if (this->Cells[x][y].Cell_Aim)
			{
				switch (Direction)
				{
				case BF_MOVE_LEFT:
				{
					if (x > 0)
					{
						this->Select(x - 1, y);
					}
				}
				break;
				case BF_MOVE_RIGHT:
				{
					if (x < OpponentGameFieldW - 1)
					{
						this->Select(x + 1, y);
					}
				}
				break;
				case BF_MOVE_UP:
				{
					if (y < OpponentGameFieldH - 1)
					{
						this->Select(x, y + 1);
					}
				}
				break;
				case BF_MOVE_DOWN:
				{
					if (y > 0)
					{
						this->Select(x, y - 1);
					}
				}
				break;
				default: return false;
				}
				return true;
			}
		}
	}
	return false;
}

bool EnemyField::ShipExists(POINT Coordinates)
{
	for (int Arrnum = 0; Arrnum < MAX_SHIPS_COUNT; Arrnum++)
		if (this->Ships[Arrnum].Decks[0].Position.x == Coordinates.x && this->Ships[Arrnum].Decks[0].Position.y == Coordinates.y)
			return true;
	return false;
}

void EnemyField::CreateShips(Engine::MODE Mode)
{
	this->ClearField();
	this->CleanShips();
	switch (Mode)
	{
	case Engine::MODE::Deploying:
	{
		Ship ship({ 0,0 }, 4);
		this->Ships[0] = ship;
		Ship ship2({ 1,0 }, 3);
		this->Ships[1] = ship2;
		Ship ship3({ 2,0 }, 3);
		this->Ships[2] = ship3;
		for (int i = 3; i < 6; i++)
		{
			Ship ship({ i,0 }, 2);
			this->Ships[i] = ship;
		}
		for (int i = 6; i < 10; i++)
		{
			Ship ship({ i,0 }, 1);
			this->Ships[i] = ship;
		}
		this->CloseNextShip();
	}
	break;
	}
}

void EnemyField::CloseNextShip()
{
	for (int i{}; i < this->Ships[engine.ShipsDeployed].Size; i++)
	{
		this->Ships[engine.ShipsDeployed].Decks[i].Open = false;
		this->Cells[this->Ships[engine.ShipsDeployed].Decks[i].Position.x][i].Open = false;
	}
}

void EnemyField::SetShipMarkers()
{
	for (int i{}; i < OpponentGameFieldW; i++)
		for (int j{}; j < OpponentGameFieldH; j++)
			this->Cells[i][j].MarkedShip = false;

	for (int Shipnum{}; Shipnum < engine.ShipsDeployed; Shipnum++)
		for (int Decknum{}; Decknum < this->Ships[Shipnum].Size; Decknum++)
			this->Cells[this->Ships[Shipnum].Decks[Decknum].Position.x][this->Ships[Shipnum].Decks[Decknum].Position.y].MarkedShip = true;
}

void EnemyField::Draw()
{
	GLuint TextureID{};
	for (int i{}; i < UserGameFieldW; i++)
	{
		for (int j{}; j < UserGameFieldH; j++)
		{
			switch (this->Cells[i][j].Open)
			{
			case true:
				if (!this->Cells[i][j].MarkedShip)
				{
					if (this->Cells[i][j].Missed && this->Cells[i][j].Cell_Aim)
						TextureID = textureManager.MissedAimTextureID;
					if (this->Cells[i][j].Missed)
						TextureID = textureManager.MissedTextureID;
					if (this->Cells[i][j].Cell_Aim)
						TextureID = textureManager.WaterAimTextureID;
					else TextureID = textureManager.WaterTextureID;
				}
				break;
			case false:
				if (this->Cells[i][j].Cell_Aim)
					TextureID = textureManager.WaterAimTextureID;
				else TextureID = textureManager.WaterTextureID;
			}
			Cells[i][j].Draw({ this->StartX + i,this->StartY + j }, TextureID);
		}
	}
	for (int Arrnum = 0; Arrnum < MAX_SHIPS_COUNT; Arrnum++)
		for (int DeckNum{}; DeckNum < this->Ships[Arrnum].Size; DeckNum++)
		{
			if (!this->Ships[Arrnum].Decks[DeckNum].Open) break;
			switch (this->Ships[Arrnum].Decks[DeckNum].integrityStatus)
			{
			case Deck::IntegrityStatus::Whole:
			{
				switch (this->Ships[Arrnum].Decks[DeckNum].Type)
				{
				case Deck::DeckType::Front:
					TextureID = textureManager.ShipFrontTextureID;
					break;
				case Deck::DeckType::Middle:
					TextureID = textureManager.ShipMiddleTextureID;
					break;
				case Deck::DeckType::Back:
					TextureID = textureManager.ShipBackTextureID;
					break;
				case Deck::DeckType::Single:
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
				if (this->Ships[Arrnum].Decks[DeckNum].Cell_Aim)
				{
					switch (this->Ships[Arrnum].Decks[DeckNum].Type)
					{
					case Deck::DeckType::Front:
						TextureID = textureManager.ShipFrontCrackedAimTextureID;
						break;
					case Deck::DeckType::Middle:
						TextureID = textureManager.ShipMiddleCrackedAimTextureID;
						break;
					case Deck::DeckType::Back:
						TextureID = textureManager.ShipBackCrackedAimTextureID;
						break;
					case Deck::DeckType::Single:
						TextureID = textureManager.SingleShipCrackedAimTextureID;
						break;
					}
				}
				else
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

void EnemyField::ClearField()
{
	for (int i{}; i < 10; i++)
		for (int j{}; j < 10; j++)
			this->Cells[i][j].Open == true;
}

void EnemyField::CleanShips()
{
	for (int i{}; i < 10; i++)
	{
		this->Ships[i].Killed = false;
		for (int j{}; j < Ships[i].Size; j++)
			this->Ships[i].Decks[j].integrityStatus = Deck::IntegrityStatus::Whole;
	}
}

/// <summary>
/// Selects a cell on the EnemyField.
/// </summary>
/// <param name="CellX: ">The X position of the cell to be selected.</param>
/// <param name="CellY: ">The Y position of the cell to be selected.</param>
void EnemyField::Select(const size_t CellX, const size_t CellY)
{
	this->Deselect();
	this->Cells[CellX][CellY].Cell_Aim = true;
}

/// <summary>
/// Deselects a cell on the EnemyField.
/// </summary>
void EnemyField::Deselect()
{
	for (int i{}; i < OpponentGameFieldW; i++)
	{
		for (int j{}; j < OpponentGameFieldH; j++)
		{
			if (this->Cells[i][j].Cell_Aim) { this->Cells[i][j].Cell_Aim = false; return; }
		}
	}
}
