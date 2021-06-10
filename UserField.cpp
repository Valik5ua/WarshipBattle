#include "UserField.h"
#include <string>

/// <summary>
/// Changes 
/// </summary>
/// <param name="coordinates"></param>
/// <returns></returns>
bool UserField::Click(POINT& coordinates)
{
	if (coordinates.x >= this->StartX && coordinates.y >= this->StartY && coordinates.x < MyGameFieldW + this->StartX && coordinates.y < MyGameFieldH + this->StartY)
	{
		coordinates.x -= this->StartX;
		coordinates.y -= this->StartY;
		return true;
	}
	return false;
}

/// <summary>
/// Draws the Field
/// </summary>
void UserField::Draw()
{
	for (int i{-1}; i <= MyGameFieldW; i++)
	{
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(StartX+i, this->StartY);
		glVertex2f(i + this->StartX, this->StartY + MyGameFieldH);
		glEnd();
	}
	for (int i{}; i <= MyGameFieldH; i++)
	{
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(this->StartX, this->StartY+i);
		glVertex2f(MyGameFieldW + this->StartX, this->StartY + i);
		glEnd();
	}
	for (int i{}; i < MyGameFieldW; i++)
	{
		for (int j{}; j < MyGameFieldH; j++)
		{
			if (Cells[i][j].Selected) glColor3f(1.0f, 1.0f, 1.0f);
			else if (Cells[i][j].Stat == Cell::Status::opened) glColor3f(0.67f, 0.67f, 0.67f);
			else glColor3f(0.0f, 0.0f, 0.0f);
			glBegin(GL_QUADS);
			glVertex2f(i + this->StartX, j + this->StartY + 0.98f);
			glVertex2f(i + this->StartX, j + this->StartY);
			glVertex2f(i + this->StartX + 0.98f, j + this->StartY);
			glVertex2f(i + this->StartX + 0.98f, j + this->StartY + 0.98f);
			glEnd();
		}
	}
}

void UserField::Select(const size_t CellX, const size_t CellY)
{
	Deselect();
	this->Cells[CellX][CellY].Selected = true;
}

void UserField::Deselect()
{
	for (int i{}; i<MyGameFieldW;i++)
	{
		for (int j{}; j < MyGameFieldH; j++)
		{
			if (this->Cells[i][j].Selected) { this->Cells[i][j].Selected = false; return; }
		}
	}
}
