#include "EnemyField.h"
#include "UserField.h"
#include "TextureManager.h"
#include "time.h"

#define DEVMODE_OFF 1
#define DEVMODE_ON  2
#define DEVMODE     DEVMODE_OFF

#if DEVMODE == DEVMODE_ON
#undef DEVMODE
#endif

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

/// <summary>
/// A function that checks whether or not a ship is located in specific loacation.
/// </summary>
/// <param name="Coordinates: ">The coordinates of the location to be checked.</param>
/// <returns>True if the ship is located in the loacation.</returns>
int EnemyField::ShipExists(const POINT Coordinates)
{
	for (int Arrnum = 0; Arrnum < MAX_SHIPS_COUNT; Arrnum++)
	{
		if (Arrnum == this->DeployingShipID) continue;
		for (int DeckCounter{}; DeckCounter < this->Ships[Arrnum].Size; DeckCounter++)
			if (this->Ships[Arrnum].Decks[DeckCounter].Position.x == Coordinates.x && this->Ships[Arrnum].Decks[DeckCounter].Position.y == Coordinates.y)
				return Arrnum;
	}
	return -1;
}

/// <summary>
/// Creates all the ships to be located in the field.
/// </summary>
/// <param name="Mode: ">The mode of the creation.</param>
void EnemyField::CreateShips(const Engine::GAMESTATUS GameStatus)
{
	this->ClearField();
	this->CleanShips();
	switch (GameStatus)
	{
	case Engine::GAMESTATUS::Deploying:
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
	case Engine::GAMESTATUS::MainGame:
	{
		switch (engine.GameMode)
		{
		case Engine::GAMEMODE::PVE:
		{
			DeployEnemyShips();
		}
		break;
		case Engine::GAMEMODE::PVP:
		{

		}
		break;
		}
	}
	}
}

/// <summary>
/// Closes the next active ship.
/// </summary>
void EnemyField::CloseNextShip()
{
	for (int i{}; i < this->Ships[engine.ShipsDeployed].Size; i++)
	{
		this->Ships[engine.ShipsDeployed].Decks[i].Open = false;
		this->Cells[this->Ships[engine.ShipsDeployed].Decks[i].Position.x][i].Open = false;
	}
}

/// <summary>
/// Sets all cells markers that tell the field that there is a ship located in the cell's position.
/// </summary>
void EnemyField::SetShipMarkers()
{
	for (int i{}; i < OpponentGameFieldW; i++)
		for (int j{}; j < OpponentGameFieldH; j++)
			this->Cells[i][j].MarkedShip = false;

	for (int Shipnum{}; Shipnum < MAX_SHIPS_COUNT; Shipnum++)
		for (int Decknum{}; Decknum < this->Ships[Shipnum].Size; Decknum++)
			this->Cells[this->Ships[Shipnum].Decks[Decknum].Position.x][this->Ships[Shipnum].Decks[Decknum].Position.y].MarkedShip = true;
}

void EnemyField::SetShipDeployableStatus(Ship& ship)
{
	for (int DeckCounter{}; DeckCounter < ship.Size; DeckCounter++)
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
			{
				if (ship.Decks[DeckCounter].Position.x + i >= 0 && ship.Decks[DeckCounter].Position.x + i < OpponentGameFieldW)
					if (ship.Decks[DeckCounter].Position.y + j >= 0 && ship.Decks[DeckCounter].Position.y + j < OpponentGameFieldH)
						if (this->ShipExists({ ship.Decks[DeckCounter].Position.x + i, ship.Decks[DeckCounter].Position.y + j }) >= 0)
						{
							ship.Deployable = false;
							return;
						}
			}
	ship.Deployable = true;
}

void EnemyField::DeployEnemyShips()
{
	const unsigned short int MinShipsOnEdge = 2;
	const unsigned short int MaxShipsOnEdge = 5;

	time_t t;
	srand((unsigned)time(&t));

	int ShipsOnEdgeToSet = rand() % (MaxShipsOnEdge - 1) + MinShipsOnEdge;
	bool SetShipOnEdge = true;
	bool PrevShipSet = true;
	int EdgePosShift{};
	int EdgePos{};

	for (int i = 0; i < MAX_SHIPS_COUNT; i++)
	{
		if (ShipsOnEdgeToSet > 0)
		{
			switch (PrevShipSet)
			{
			case true:
			{
				if (i >= MinShipsOnEdge)
				{
					if ((((rand() % 10) + 1) % 2) == 1)
					{
						EdgePosShift = rand() % (10 - this->Ships[i].Size + 1);
						EdgePos = rand() % 4;
						SetShipOnEdge = true;
					}
					else if (ShipsOnEdgeToSet >= MAX_SHIPS_COUNT - i)
					{
						EdgePosShift = rand() % (10 - this->Ships[i].Size + 1);
						EdgePos = rand() % 4;
						SetShipOnEdge = true;
					}
					else SetShipOnEdge = false;
				}
				else
				{
					EdgePosShift = rand() % (10 - this->Ships[i].Size + 1);
					EdgePos = rand() % 4;
					SetShipOnEdge = true;
				}
			}
			break;
			case false:
			{
				i--;
				SetShipOnEdge = true;
				if (EdgePos != 3) EdgePos++;
				else EdgePos = 0;
				EdgePosShift = rand() % (10 - this->Ships[i].Size + 1);
				PrevShipSet = true;
			}
			break;
			}
		}
		else
		{
			SetShipOnEdge = false;
		}

		this->DeployingShipID = i;
		switch (SetShipOnEdge)
		{
		case true:
		{
			switch (EdgePos)
			{
			case 0:
			{
				this->Ships[i].Rotated = true;
				for (int DeckNum = 0; DeckNum < this->Ships[i].Size; DeckNum++)
				{
					this->Ships[i].Decks[DeckNum].Position.x = EdgePosShift + DeckNum;
					this->Ships[i].Decks[DeckNum].Position.y = 0;
				}
				this->SetShipDeployableStatus(this->Ships[i]);
				if (!this->Ships[i].Deployable)
				{
					PrevShipSet = false;
					for (int ShipPos = 0; ShipPos <= OpponentGameFieldW - this->Ships[i].Size; ShipPos++)
					{
						for (int DeckNum = 0; DeckNum < this->Ships[i].Size; DeckNum++)
						{
							this->Ships[i].Decks[DeckNum].Position.x = ShipPos + DeckNum;
							this->Ships[i].Decks[DeckNum].Position.y = 0;
						}
						this->SetShipDeployableStatus(this->Ships[i]);
						if (this->Ships[i].Deployable)
						{
							PrevShipSet = true;
							ShipsOnEdgeToSet--;
							break;
						}
					}
				}
				else
				{
					PrevShipSet = true;
					ShipsOnEdgeToSet--;
				}
			}
			break;
			case 1:
			{
				this->Ships[i].Rotated = false;
				for (int DeckNum = 0; DeckNum < this->Ships[i].Size; DeckNum++)
				{
					this->Ships[i].Decks[DeckNum].Position.x = 0;
					this->Ships[i].Decks[DeckNum].Position.y = EdgePosShift + DeckNum;
				}
				this->SetShipDeployableStatus(this->Ships[i]);
				if (!this->Ships[i].Deployable)
				{
					PrevShipSet = false;
					for (int ShipPos = 0; ShipPos <= OpponentGameFieldH - this->Ships[i].Size; ShipPos++)
					{
						for (int DeckNum = 0; DeckNum < this->Ships[i].Size; DeckNum++)
						{
							this->Ships[i].Decks[DeckNum].Position.x = 0;
							this->Ships[i].Decks[DeckNum].Position.y = ShipPos + DeckNum;
						}
						this->SetShipDeployableStatus(this->Ships[i]);
						if (this->Ships[i].Deployable)
						{
							PrevShipSet = true;
							ShipsOnEdgeToSet--;
							break;
						}
					}
				}
				else
				{
					PrevShipSet = true;
					ShipsOnEdgeToSet--;
				}
			}
			break;
			case 2:
			{
				this->Ships[i].Rotated = true;
				for (int DeckNum = 0; DeckNum < this->Ships[i].Size; DeckNum++)
				{
					this->Ships[i].Decks[DeckNum].Position.x = EdgePosShift + DeckNum;
					this->Ships[i].Decks[DeckNum].Position.y = OpponentGameFieldH - 1;
				}
				this->SetShipDeployableStatus(this->Ships[i]);
				if (!this->Ships[i].Deployable)
				{
					PrevShipSet = false;
					for (int ShipPos = 0; ShipPos <= OpponentGameFieldW - this->Ships[i].Size; ShipPos++)
					{
						for (int DeckNum = 0; DeckNum < this->Ships[i].Size; DeckNum++)
						{
							this->Ships[i].Decks[DeckNum].Position.x = ShipPos + DeckNum;
							this->Ships[i].Decks[DeckNum].Position.y = OpponentGameFieldH - 1;
						}
						this->SetShipDeployableStatus(this->Ships[i]);
						if (this->Ships[i].Deployable)
						{
							PrevShipSet = true;
							ShipsOnEdgeToSet--;
							break;
						}
					}
				}
				else
				{
					PrevShipSet = true;
					ShipsOnEdgeToSet--;
				}
			}
			break;
			case 3:
			{
				this->Ships[i].Rotated = false;
				for (int DeckNum = 0; DeckNum < this->Ships[i].Size; DeckNum++)
				{
					this->Ships[i].Decks[DeckNum].Position.x = OpponentGameFieldW - 1;
					this->Ships[i].Decks[DeckNum].Position.y = EdgePosShift + DeckNum;
				}
				this->SetShipDeployableStatus(this->Ships[i]);
				if (!this->Ships[i].Deployable)
				{
					PrevShipSet = false;
					for (int ShipPos = 0; ShipPos <= OpponentGameFieldH - this->Ships[i].Size; ShipPos++)
					{
						for (int DeckNum = 0; DeckNum < this->Ships[i].Size; DeckNum++)
						{
							this->Ships[i].Decks[DeckNum].Position.x = OpponentGameFieldW - 1;
							this->Ships[i].Decks[DeckNum].Position.y = ShipPos + DeckNum;
						}
						this->SetShipDeployableStatus(this->Ships[i]);
						if (this->Ships[i].Deployable)
						{
							PrevShipSet = true;
							ShipsOnEdgeToSet--;
							break;
						}
					}
				}
				else
				{
					PrevShipSet = true;
					ShipsOnEdgeToSet--;
				}
			}
			break;
			}
		}
		break;
		case false:
		{
			this->Ships[i].Rotated = (((rand() % 10) + 1) % 2);
			switch (this->Ships[i].Rotated)
			{
			case true:
			{
				do
				{
					POINT RandPoint = { rand() % (OpponentGameFieldW - this->Ships[i].Size + 1) ,(rand() % (OpponentGameFieldH - 2)) + 1 };
					for (int DeckNum = 0; DeckNum < this->Ships[i].Size; DeckNum++)
					{
						this->Ships[i].Decks[DeckNum].Position.x = RandPoint.x + DeckNum;
						this->Ships[i].Decks[DeckNum].Position.y = RandPoint.y;
					}
					this->SetShipDeployableStatus(this->Ships[i]);
				} while (!this->Ships[i].Deployable);
			}
			break;
			case false:
			{
				do
				{
					POINT RandPoint = { (rand() % (OpponentGameFieldW - 2)) + 1 ,rand() % (OpponentGameFieldH - this->Ships[i].Size + 1) };
					for (int DeckNum = 0; DeckNum < this->Ships[i].Size; DeckNum++)
					{
						this->Ships[i].Decks[DeckNum].Position.x = RandPoint.x;
						this->Ships[i].Decks[DeckNum].Position.y = RandPoint.y + DeckNum;
					}
					this->SetShipDeployableStatus(this->Ships[i]);
				} while (!this->Ships[i].Deployable);
			}
			break;
			}
		}
		break;
		}
#ifndef DEVMODE
		for (int DeckNum = 0; DeckNum < this->Ships[i].Size; DeckNum++)
			this->Ships[i].Decks[DeckNum].Open = false;
#endif // !DEVMODE
	}
	SetShipMarkers();
}

/// <summary>
/// Clears the field of any markers set before.
/// </summary>
void EnemyField::ClearField()
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
void EnemyField::CleanShips()
{
	for (int i{}; i < 10; i++)
	{
		this->Ships[i].Killed = false;
		for (int j{}; j < Ships[i].Size; j++)
		{
			this->Ships[i].Decks[j].integrityStatus = Deck::IntegrityStatus::Whole;
			this->Ships[i].Decks[j].Position = { -1,-1 };
			this->Ships[i].Decks[j].Open = true;
		}
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

void EnemyField::RandomSelect(const size_t, const size_t)
{
	int RandX = rand() % 10;
	int RandY = rand() % 10;
	while (this->Cells[RandX][RandY].MarkedShip || this->Cells[RandX][RandY].Missed)
	{
		RandX = rand() % 10;
		RandY = rand() % 10;
	}
	this->Select(RandX, RandY);
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
			if (this->Cells[i][j].Cell_Aim) { this->Cells[i][j].Cell_Aim = false;}
		}
	}
}

/// <summary>
/// Draws the field.
/// </summary>
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
					if (this->Cells[i][j].Missed && this->Cells[i][j].Cell_Aim)
						TextureID = textureManager.MissedAimTextureID;
					if (this->Cells[i][j].Missed)
						TextureID = textureManager.MissedTextureID;
					if (this->Cells[i][j].Cell_Aim)
						TextureID = textureManager.WaterAimTextureID;
					else TextureID = textureManager.WaterTextureID;
				break;
			case false:
				if (this->Cells[i][j].Cell_Aim)
					TextureID = textureManager.WaterAimTextureID;
				else TextureID = textureManager.WaterTextureID;
			}
			Cells[i][j].Draw({ this->StartX + i, this->StartY + j }, TextureID);
		}
	}
	for (int Arrnum = 0; Arrnum < MAX_SHIPS_COUNT; Arrnum++)
		for (int DeckNum{}; DeckNum < this->Ships[Arrnum].Size; DeckNum++)
		{
			if (!Ships[Arrnum].Decks[DeckNum].Open) break;
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
