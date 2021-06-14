#include "ButtonField.h"

ButtonField::ButtonField(int StartX, int StartY)
{
	this->StartX = StartX;
	this->StartY = StartY;
	this->Cells[0][0].ButtonID = BF_MOVE_LEFT;
	this->Cells[0][1].ButtonID = BF_MOVE_LEFT;

	this->Cells[0][2].ButtonID = BF_MOVE_UP;

	this->Cells[1][0].ButtonID = BF_MOVE_DOWN;

	this->Cells[1][1].ButtonID = BF_ROTATE;

	this->Cells[1][2].ButtonID = BF_MOVE_UP;

	this->Cells[2][0].ButtonID = BF_MOVE_DOWN;

	this->Cells[2][1].ButtonID = BF_MOVE_RIGHT;
	this->Cells[2][2].ButtonID = BF_MOVE_RIGHT;

	this->Cells[3][0].ButtonID = BF_FIRE;
	this->Cells[3][1].ButtonID = BF_FIRE;
	this->Cells[3][2].ButtonID = BF_FIRE;
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
			glColor3f(0.0f, 0.0f, 0.0f);
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
			glVertex2f(i + this->StartX + 0.98f, j + this->StartY);
			glVertex2f(i + this->StartX + 0.98f, j + this->StartY + 0.98f);
			glVertex2f(i + this->StartX, j + this->StartY + 0.98f);
			glEnd();
		}
}
