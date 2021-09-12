#include "Ship.h"

Ship::Ship(POINT Position, int Size) :Deployable(false), Deployed(false), Killed(false), Rotated(false)
{
	for (int i{}; i < Size; i++)
	{
		this->Decks[i].Position.x = Position.x;
		this->Decks[i].Position.y = Position.y + i;
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

//bool Ship::Click(POINT Coordinates)
//{
//    return false;
//}
