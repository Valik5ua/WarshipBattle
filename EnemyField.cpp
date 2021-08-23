#include "EnemyField.h"
#include "TextureManager.h"

extern TextureManager textureManager;

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
/// Draws the EnemyField.
/// </summary>
void EnemyField::Draw()
{
	for (int i{}; i < OpponentGameFieldW; i++)
	{
		for (int j{}; j < OpponentGameFieldH; j++)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureManager.WaterTextureID);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
			glTexCoord2d(0, 1.f); glVertex2f(i + this->StartX, j + this->StartY + 1.0f);
			glTexCoord2d(1.f, 1.f); glVertex2f(i + this->StartX + 1.0f, j + this->StartY + 1.0f);
			glTexCoord2d(1.f, 0); glVertex2f(i + this->StartX + 1.0f, j + this->StartY);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			if (this->Cells[i][j].Selected)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textureManager.WaterAimTextureID);
				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(0, 1.f); glVertex2f(i + this->StartX, j + this->StartY + 1.0f);
				glTexCoord2d(1.f, 1.f); glVertex2f(i + this->StartX + 1.0f, j + this->StartY + 1.0f);
				glTexCoord2d(1.f, 0); glVertex2f(i + this->StartX + 1.0f, j + this->StartY);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
		}
	}
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
			if (this->Cells[x][y].Selected)
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
/// Selects a cell on the EnemyField.
/// </summary>
/// <param name="CellX: ">The X position of the cell to be selected.</param>
/// <param name="CellY: ">The Y position of the cell to be selected.</param>
void EnemyField::Select(const size_t CellX, const size_t CellY)
{
	this->Deselect();
	this->Cells[CellX][CellY].Selected = true;
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
			if (this->Cells[i][j].Selected) { this->Cells[i][j].Selected = false; return; }
		}
	}
}
