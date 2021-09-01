#include "Field.h"
#include "TextureManager.h"

extern TextureManager textureManager;
extern Ship Ships[10];

/// <summary>
/// The default constructor fo the Field class.
/// </summary>
Field::Field() :StartX(0), StartY(0)
{
}

/// <summary>
/// Checks if a ship exists around the given coordinates.
/// </summary>
/// <param name="Coordinates: ">The coordinates to be checked around.</param>
/// <returns>Wether or not a ship exists around the given coordinates.</returns>
bool Field::ShipExistsAround(const POINT Coordinates, Ship* ship)
{
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			for (int Arrnum = 0; Arrnum < 10; Arrnum++)
				if (Ships[Arrnum].StartPos.x == Coordinates.x + i &&
					Ships[Arrnum].StartPos.y == Coordinates.y + j)
				{
					*ship = Ships[Arrnum];
					return true;
				}
	return false;
}

/// <summary>
/// Checks if a ship exists around a ship.
/// </summary>
/// <param name="ClickedShip: ">The ship to be checked around.</param>
/// <returns>Wether or not a ship exists around the selected ship.</returns>
bool Field::ShipExistsAround(Ship& ClickedShip)
{
	for (int i = 0; i < ClickedShip.Size; i++)
	{
		POINT DefaultValue{};
		ClickedShip.StartPos.x += i;
		if (ShipExistsAround(ClickedShip.StartPos)) { ClickedShip.StartPos.x -= i; return true; }
		ClickedShip.StartPos.x -= i;
	}
	return false;
}

/// <summary>
/// Draws the Field.
/// </summary>
void Field::Draw()
{
	GLuint TextureID{};
	for (int i{}; i < MyGameFieldW; i++)
	{
		for (int j{}; j < MyGameFieldH; j++)
		{
			Ship FoundShip;
			if (ShipExistsAround({ i,j }, &FoundShip) && FoundShip.StartPos.x  == i && FoundShip.StartPos.x == j)
			{
				switch (FoundShip.Decks[0].integrityStatus)
				{
				case Deck::IntegrityStatus::Whole:
				{
					if (FoundShip.Decks[0].Cell_Aim)
					{
						switch (FoundShip.Decks[0].Type)
						{
						case Deck::DeckType::Front:
							TextureID = textureManager.ShipFrontAimTextureID;
							break;
						case Deck::DeckType::Middle:
							TextureID = textureManager.ShipMiddleAimTextureID;
							break;
						case Deck::DeckType::Back:
							TextureID = textureManager.ShipBackAimTextureID;
							break;
						case Deck::DeckType::Single:
							TextureID = textureManager.SingleShipAimTextureID;
							break;
						}
					}
					else
					{
						switch (FoundShip.Decks[0].Type)
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
				}
				break;
				case Deck::IntegrityStatus::Damaged:
				{
					if (FoundShip.Decks[0].Cell_Aim)
					{
						switch (FoundShip.Decks[0].Type)
						{
						case Deck::DeckType::Front:
							TextureID = textureManager.ShipFrontAfireAimTextureID;
							break;
						case Deck::DeckType::Middle:
							TextureID = textureManager.ShipMiddleAfireAimTextureID;
							break;
						case Deck::DeckType::Back:
							TextureID = textureManager.ShipBackAfireAimTextureID;
							break;
						}
					}
					else
					{
						switch (FoundShip.Decks[0].Type)
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
				}
				break;
				case Deck::IntegrityStatus::Killed:
				{
					if (FoundShip.Decks[0].Cell_Aim)
					{
						switch (FoundShip.Decks[0].Type)
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
						switch (FoundShip.Decks[0].Type)
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
			}
			else TextureID = textureManager.WaterTextureID;

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, TextureID);

			glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
			glTexCoord2d(1.f, 0); glVertex2f(i + 1.f + this->StartX, j + this->StartY);
			glTexCoord2d(1.f, 1.f); glVertex2f(i + 1.f + this->StartX, j + 1.f + this->StartY);
			glTexCoord2d(0, 1.f); glVertex2f(i + this->StartX, j + 1.f + this->StartY);
			glEnd();

			glDisable(GL_TEXTURE_2D);
		}
	}
}
