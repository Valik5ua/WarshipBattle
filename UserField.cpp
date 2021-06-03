#include "UserField.h"
#include <string>

extern HWND hwnd;
extern const int MyGameFieldW;
extern const int MyGameFieldH;

bool UserField::Click(float GLmouseX, float GLmouseY)
{
	if (GLmouseX - StartX < 0 || GLmouseY - StartY < 0 || GLmouseX - StartX >= MyGameFieldW || GLmouseY - StartY >= MyGameFieldH) return false;
	Cells[(int)GLmouseX - StartX][(int)GLmouseY - StartY].Stat=Cell::Status::opened;
	std::string msg = "The primary user field was clicked! Cell: (";
	msg += std::to_string((int)GLmouseX - StartX);
	msg += ",";
	msg += std::to_string((int)GLmouseY - StartY);
	msg += ")";
	MessageBoxA(hwnd, msg.c_str(), "user field was clicked", NULL);
	return true;
}

void UserField::Draw()
{
	for (int i{-1}; i <= MyGameFieldW; i++)
	{
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(StartX+i, StartY);
		glVertex2f(i + StartX, StartY + MyGameFieldH);
		glEnd();
	}
	for (int i{}; i <= MyGameFieldH; i++)
	{
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(StartX, StartY+i);
		glVertex2f(MyGameFieldW + StartX, StartY + i);
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
