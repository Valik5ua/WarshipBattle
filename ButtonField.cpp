#include "ButtonField.h"

ButtonField::ButtonField(int StartX, int StartY)
{
	this->StartX = StartX;
	this->StartY = StartY;
}

bool ButtonField::Click(POINT& coordinates)
{
	if (coordinates.x >= this->StartX && coordinates.y >= this->StartY && coordinates.x < ButtonFieldW + this->StartX && coordinates.y < ButtonFieldH + this->StartY)
	{
		coordinates.x -= this->StartX;
		coordinates.y -= this->StartY;
		return true;
	}
	return false;
}

void ButtonField::Draw()
{
	for (int i = StartX; i < ButtonFieldW + StartX; i++)
	{
		for (int j = StartY; j < ButtonFieldH + StartY; j++)
		{
			glBegin(GL_LINES);
			glVertex2f(i, j);
			glVertex2f(i + 0.98f, j);

			glVertex2f(i + 0.98f, j);
			glVertex2f(i + 0.98f, j + 0.98f);

			glVertex2f(i + 0.98f, j + 0.98f);
			glVertex2f(i, j + 0.98f);

			glVertex2f(i, j + 0.98f);
			glVertex2f(i, j);
			glEnd();
		}
	}
	for (int i = 0; i < ButtonFieldW; i++)
		for (int j = 0; j < ButtonFieldH; j++)
		{
			if (Cells[i][j].Selected) glColor3f(1.0f, 1.0f, 1.0f);
			else glColor3f(0.0f, 0.0f, 0.0f);
			glBegin(GL_QUADS);
			glVertex2f(i + this->StartX, j + this->StartY);
			glVertex2f(i + this->StartX + 1, j + this->StartY);
			glVertex2f(i + this->StartX + 1, j + this->StartY + 1);
			glVertex2f(i + this->StartX, j + this->StartY + 1);
			glEnd();
		}
}

void ButtonField::Select(const size_t CellX, const size_t CellY)
{
	Deselect();
	Cells[CellX][CellY].Selected = true;
	if (CellX == 0 && CellY == 0) Cells[CellX][CellY + 1].Selected = true;
	if (CellX == 0 && CellY == 1) Cells[CellX][CellY - 1].Selected = true;
	if (CellX == 1 && CellY == 0) Cells[CellX + 1][CellY].Selected = true;
	if (CellX == 2 && CellY == 0) Cells[CellX - 1][CellY].Selected = true;
	if (CellX == 2 && CellY == 1) Cells[CellX][CellY + 1].Selected = true;
	if (CellX == 2 && CellY == 2) Cells[CellX][CellY - 1].Selected = true;
	if (CellX == 0 && CellY == 2) Cells[CellX + 1][CellY].Selected = true;
	if (CellX == 1 && CellY == 2) Cells[CellX - 1][CellY].Selected = true;
	if (CellX == 3 && CellY == 0) { Cells[CellX][CellY + 1].Selected = true; Cells[CellX][CellY + 2].Selected = true; }
	if (CellX == 3 && CellY == 1) { Cells[CellX][CellY - 1].Selected = true; Cells[CellX][CellY + 1].Selected = true; }
	if (CellX == 3 && CellY == 2) { Cells[CellX][CellY - 2].Selected = true; Cells[CellX][CellY - 1].Selected = true; }
}

void ButtonField::Deselect()
{
	for (int i{}; i < ButtonFieldW; i++)
	{
		for (int j{}; j < ButtonFieldH; j++)
		{
			if (this->Cells[i][j].Selected) this->Cells[i][j].Selected = false;
		}
	}
}
