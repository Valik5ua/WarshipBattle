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

void EnemyField::CreateShips(Engine::MODE Mode)
{
	this->ClearField();
	this->CleanShips();
	switch (Mode)
	{
	case Engine::MODE::Deploying:
	{
		this->Ships[0].SetSize(4);
		this->Ships[1].SetSize(3);
		this->Ships[2].SetSize(3);
		for (int i = 3; i < 6; i++)
			this->Ships[i].SetSize(2);
		for (int i = 6; i < 10; i++)
			this->Ships[i].SetSize(1);

		for (int i{}; i < 10; i++)
		{
			this->Ships[i].StartPos.x += i;
			this->Ships[i].Deployed = false;
		}
	}
	break;
	}
}

void EnemyField::CloseShip()
{
	for (int i{}; i < this->Ships[engine.ShipsDeployed].Size; i++)
		this->Ships[i].Decks[i].Open = false;
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
