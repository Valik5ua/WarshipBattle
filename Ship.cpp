#include "Ship.h"
#include "UserField.h"

extern UserField userField;

/// <summary>
/// A detailed consturctor for the Ship class.
/// </summary>
/// <param name="Position: ">The position of the ship.</param>
/// <param name="Size: ">The size of the ship.</param>
Ship::Ship(const POINT Position, const size_t Size) :Deployable(false), Deployed(false), Killed(false), Rotated(false)
{
	for (int i{}; i < Size; i++)
	{
		this->Decks[i].Position.x = Position.x;
		this->Decks[i].Position.y = Position.y + i;
		this->Decks[i].Open = true;
	}
	this->Size = Size;
	switch (Size)
	{
	case 1:
		this->Decks[0].Type = Deck::DeckType::Single;
		break;
	case 2:
		this->Decks[0].Type = Deck::DeckType::Back;
		this->Decks[1].Type = Deck::DeckType::Front;
		break;
	case 3:
		this->Decks[0].Type = Deck::DeckType::Back;
		this->Decks[1].Type = Deck::DeckType::Middle;
		this->Decks[2].Type = Deck::DeckType::Front;
		break;
	case 4:
		this->Decks[0].Type = Deck::DeckType::Back;
		this->Decks[1].Type = Deck::DeckType::Middle;
		this->Decks[2].Type = Deck::DeckType::Middle;
		this->Decks[3].Type = Deck::DeckType::Front;
		break;
	}
}

void Ship::SetDamageToDeck(POINT CoordinatesOfDeck)
{
	for (int Deck = 0; Deck < this->Size; Deck++)
	{
		if (this->Decks[Deck].Position.x == CoordinatesOfDeck.x && this->Decks[Deck].Position.y == CoordinatesOfDeck.y)
		{
			this->Decks[Deck].integrityStatus = Deck::IntegrityStatus::Damaged;
			break;
		}
	}
	int DamagedDecks = 0;
	for (int Deck = 0; Deck < this->Size; Deck++)
	{
		if (this->Decks[Deck].integrityStatus == Deck::IntegrityStatus::Damaged) DamagedDecks++;
	}
	if (DamagedDecks == this->Size)
	{
		this->Killed = true;
		for (int Deck = 0; Deck < this->Size; Deck++)
		{
			this->Decks[Deck].integrityStatus = Deck::IntegrityStatus::Killed;
		}
	}
}
