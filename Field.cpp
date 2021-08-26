#include "Field.h"

Ship* Ships[10];

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
bool Field::ShipExistsAround(const POINT Coordinates)
{
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			for (int Arrnum = 0; Arrnum < 10; Arrnum++)
				if (Ships[Arrnum]->StartPos.x==Coordinates.x+i && 
					Ships[Arrnum]->StartPos.y == Coordinates.y + j)
				return true;
	return false;
}

/// <summary>
/// Checks if a ship exists around a ship.
/// </summary>
/// <param name="ClickedShip: ">The ship to be checked around.</param>
/// <returns>Wether or not a ship exists around the selected ship.</returns>
bool Field::ShipExistsAround(Ship& ClickedShip)
{
	for (int i = 0; i < ClickedShip.GetSize(); i++)
	{
		ClickedShip.StartPos.x += i;
		if (ShipExistsAround(ClickedShip.StartPos)) { ClickedShip.StartPos.x -= i; return true; }
		ClickedShip.StartPos.x -= i;
	}
	return false;
}
