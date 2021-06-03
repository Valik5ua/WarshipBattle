#include "UserField.h"
#include "Engine.h"
#include <string>

extern Engine _Engine;

bool UserField::Click(POINT coordinates)
{
	coordinates.x -= this->StartX;
	coordinates.y -= this->StartY;
	if (coordinates.x < 0 || coordinates.y < 0 || coordinates.x >= MyGameFieldW || coordinates.y >= MyGameFieldH) return false;
	Cells[coordinates.x][coordinates.y].Stat = Cell::Status::opened;
	_Engine.ClickEvent(this, coordinates);
}

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
			if(Cells[i][j].Stat==Cell::Status::opened) glColor3f(1.0f, 1.0f, 1.0f);
			else glColor3f(0.0f, 0.0f, 0.0f);
			glBegin(GL_QUADS);
			glVertex2f(i + StartX, j + StartY + 0.98f);
			glVertex2f(i + StartX, j + StartY);
			glVertex2f(i + StartX + 0.98f, j + StartY);
			glVertex2f(i + StartX + 0.98f, j + StartY + 0.98f);
			glEnd();
		}
	}
}
