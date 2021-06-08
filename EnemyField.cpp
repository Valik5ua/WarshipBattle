#include "EnemyField.h"
#include <string>

bool EnemyField::Click(POINT& coordinates)
{
	if (coordinates.x >= this->StartX && coordinates.y >= this->StartY && coordinates.x < MyGameFieldW + this->StartX && coordinates.y < MyGameFieldH + this->StartY)
	{
		coordinates.x -= this->StartX;
		coordinates.y -= this->StartY;
		return true;
	}
	return false;
}

void EnemyField::Draw()
{
	for (int i{}; i <= OpponentGameFieldW; i++)
	{
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(this->StartX + i, this->StartY);
		glVertex2f(i + this->StartX, this->StartY + OpponentGameFieldH);
		glEnd();
	}
	for (int i{}; i <= OpponentGameFieldH; i++)
	{
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(this->StartX, this->StartY + i);
		glVertex2f(OpponentGameFieldW + this->StartX, this->StartY + i);
		glEnd();
	}
	for (int i{}; i < OpponentGameFieldW; i++)
	{
		for (int j{}; j < OpponentGameFieldH; j++)
		{
			if (Cells[i][j].Stat == Cell::Status::opened) glColor3f(1.0f, 1.0f, 1.0f);
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
