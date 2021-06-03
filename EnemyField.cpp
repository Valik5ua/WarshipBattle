#include "EnemyField.h"
#include <string>
#include "Engine.h"

extern Engine _Engine;

bool EnemyField::Click(POINT coordinates)
{
	coordinates.x -= this->StartX;
	coordinates.y -= this->StartY;
	if (coordinates.x < 0 || coordinates.y < 0 || coordinates.x >= MyGameFieldW || coordinates.y >= MyGameFieldH) return false;
	Cells[coordinates.x][coordinates.y].Stat = Cell::Status::opened;
	_Engine.ClickEvent(this, coordinates);
}

void EnemyField::Draw()
{
	for (int i{}; i <= OpponentGameFieldW; i++)
	{
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(StartX + i, StartY);
		glVertex2f(i + StartX, StartY + OpponentGameFieldH);
		glEnd();
	}
	for (int i{}; i <= OpponentGameFieldH; i++)
	{
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(StartX, StartY + i);
		glVertex2f(OpponentGameFieldW + StartX, StartY + i);
		glEnd();
	}
	for (int i{}; i < OpponentGameFieldW; i++)
	{
		for (int j{}; j < OpponentGameFieldH; j++)
		{
			if (Cells[i][j].Stat == Cell::Status::opened) glColor3f(1.0f, 1.0f, 1.0f);
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
