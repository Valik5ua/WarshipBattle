#include "Ship.h"

Ship::Ship(POINT StartPos, int Size) :StartPos(StartPos) { SetSize(Size); }

void Ship::SetSize(int Size)
{
	this->Size = Size;
	switch (Size)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	}
}

//bool Ship::Click(POINT Coordinates)
//{
//    return false;
//}
