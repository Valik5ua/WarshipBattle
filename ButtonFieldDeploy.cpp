#include "ButtonFieldDeploy.h"

extern GLuint	Btn_RotateTextureID;
extern GLuint	Btn_DownTextureID;
extern GLuint	Btn_UpTextureID;
extern GLuint	Btn_LeftTextureID;
extern GLuint	Btn_RightTextureID;
extern GLuint	Btn_DeployTextureID;
extern GLuint	WaterTextureID;

ButtonFieldDeploy::ButtonFieldDeploy(int StartX, int StartY)
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

	this->Cells[3][0].ButtonID = BF_DEPLOY;
	this->Cells[3][1].ButtonID = BF_DEPLOY;
	this->Cells[3][2].ButtonID = BF_DEPLOY;
}

bool ButtonFieldDeploy::Click(POINT& coordinates)
{
	if (coordinates.x >= this->StartX && coordinates.y >= this->StartY && coordinates.x < ButtonFieldW + this->StartX && coordinates.y < ButtonFieldH + this->StartY)
	{
		coordinates.x -= this->StartX;
		coordinates.y -= this->StartY;
		return true;
	}
	return false;
}

void ButtonFieldDeploy::Draw()
{
	for (int i = 0; i < ButtonFieldW; i++)
		for (int j = 0; j < ButtonFieldH; j++)
		{
			if (this->Cells[i][j].ButtonID == BF_MOVE_UP && this->Cells[i + 1][j].ButtonID == BF_MOVE_UP)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, Btn_UpTextureID);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(1, 0); glVertex2f(i + this->StartX + 1.98f, j + this->StartY);
				glTexCoord2d(1, 1); glVertex2f(i + this->StartX + 1.98f, j + this->StartY + 0.98f);
				glTexCoord2d(0, 1); glVertex2f(i + this->StartX, j + this->StartY + 0.98f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			if (this->Cells[i][j].ButtonID == BF_MOVE_DOWN && this->Cells[i + 1][j].ButtonID == BF_MOVE_DOWN)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, Btn_DownTextureID);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(1, 0); glVertex2f(i + this->StartX + 1.98f, j + this->StartY);
				glTexCoord2d(1, 1); glVertex2f(i + this->StartX + 1.98f, j + this->StartY + 0.98f);
				glTexCoord2d(0, 1); glVertex2f(i + this->StartX, j + this->StartY + 0.98f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			if (this->Cells[i][j].ButtonID == BF_MOVE_LEFT && this->Cells[i][j + 1].ButtonID == BF_MOVE_LEFT)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, Btn_LeftTextureID);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(1, 0); glVertex2f(i + this->StartX + 0.98f, j + this->StartY);
				glTexCoord2d(1, 1); glVertex2f(i + this->StartX + 0.98f, j + this->StartY + 1.98f);
				glTexCoord2d(0, 1); glVertex2f(i + this->StartX, j + this->StartY + 1.98f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			if (this->Cells[i][j].ButtonID == BF_MOVE_RIGHT && this->Cells[i][j + 1].ButtonID == BF_MOVE_RIGHT)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, Btn_RightTextureID);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(1, 0); glVertex2f(i + this->StartX + 0.98f, j + this->StartY);
				glTexCoord2d(1, 1); glVertex2f(i + this->StartX + 0.98f, j + this->StartY + 1.98f);
				glTexCoord2d(0, 1); glVertex2f(i + this->StartX, j + this->StartY + 1.98f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			if (this->Cells[i][j].ButtonID == BF_ROTATE)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, Btn_RotateTextureID);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(1, 0); glVertex2f(i + this->StartX + 0.98f, j + this->StartY);
				glTexCoord2d(1, 1); glVertex2f(i + this->StartX + 0.98f, j + this->StartY + 0.98f);
				glTexCoord2d(0, 1); glVertex2f(i + this->StartX, j + this->StartY + 0.98f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			if (this->Cells[i][j].ButtonID == BF_DEPLOY && this->Cells[i][j + 1].ButtonID == BF_DEPLOY && this->Cells[i][j + 2].ButtonID == BF_DEPLOY)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, Btn_DeployTextureID);

				glBegin(GL_QUADS);
				glTexCoord2d(0, 0); glVertex2f(i + this->StartX, j + this->StartY);
				glTexCoord2d(1, 0); glVertex2f(i + this->StartX + 0.98f, j + this->StartY);
				glTexCoord2d(1, 1); glVertex2f(i + this->StartX + 0.98f, j + this->StartY + 2.98f);
				glTexCoord2d(0, 1); glVertex2f(i + this->StartX, j + this->StartY + 2.98f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
				return;
			}
		}
}