#pragma once
#include "GL\glew.h"

class TextureManager
{
public:
	void LoadAllTextures();
private:
	bool LoadTexture(char* FileName, GLuint& Texture_ID);
public:
	//Texture IDs

	GLuint	ShipFrontTextureID = 1;
	GLuint	ShipMiddleTextureID = 2;
	GLuint	ShipBackTextureID = 3;

	GLuint	ShipFrontAimTextureID = 4;
	GLuint	ShipMiddleAimTextureID = 5;
	GLuint	ShipBackAimTextureID = 6;

	GLuint	ShipFrontAfireTextureID = 7;
	GLuint	ShipMiddleAfireTextureID = 8;
	GLuint	ShipBackAfireTextureID = 9;

	GLuint	ShipFrontAfireAimTextureID = 10;
	GLuint	ShipMiddleAfireAimTextureID = 11;
	GLuint	ShipBackAfireAimTextureID = 12;

	GLuint	ShipFrontCrackedTextureID = 13;
	GLuint	ShipMiddleCrackedTextureID = 14;
	GLuint	ShipBackCrackedTextureID = 15;

	GLuint	ShipFrontCrackedAimTextureID = 16;
	GLuint	ShipMiddleCrackedAimTextureID = 17;
	GLuint	ShipBackCrackedAimTextureID = 18;

	GLuint   SingleShipTextureID = 19;
	GLuint   SingleShipAimTextureID = 20;
	GLuint   SingleShipCrackedTextureID = 21;
	GLuint   SingleShipCrackedAimTextureID = 22;

	GLuint  Btn_RotateTextureID = 23;
	GLuint	Btn_RandomAimTextureID = 24;
	GLuint	WaterAimTextureID = 25;

	GLuint	Btn_DownTextureID = 26;
	GLuint	Btn_UpTextureID = 27;
	GLuint	Btn_LeftTextureID = 28;
	GLuint	Btn_RightTextureID = 29;

	GLuint	Btn_FireTextureID = 30;
	GLuint	Btn_DeployTextureID = 31;

	GLuint  Btn_DisconnectTextureID = 32;
	GLuint  Btn_ConnectTextureID = 33;
	GLuint  Btn_CancelTextureID = 34;

	GLuint	WaterTextureID = 35;
	GLuint	MissTextureID = 36;
	GLuint	MissAimTextureID = 37;
};
