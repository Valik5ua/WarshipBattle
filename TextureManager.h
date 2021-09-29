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

	GLuint	ShipFrontTextureID;
	GLuint	ShipMiddleTextureID;
	GLuint	ShipBackTextureID;

	GLuint	ShipFrontDeployableTextureID;
	GLuint	ShipMiddleDeployableTextureID;
	GLuint	ShipBackDeployableTextureID;
	
	GLuint	ShipFrontUndeployableTextureID;
	GLuint	ShipMiddleUndeployableTextureID;
	GLuint	ShipBackUndeployableTextureID;

	GLuint	ShipFrontAfireTextureID;
	GLuint	ShipMiddleAfireTextureID;
	GLuint	ShipBackAfireTextureID;

	GLuint	ShipFrontCrackedTextureID;
	GLuint	ShipMiddleCrackedTextureID;
	GLuint	ShipBackCrackedTextureID;

	GLuint	ShipFrontCrackedAimTextureID;
	GLuint	ShipMiddleCrackedAimTextureID;
	GLuint	ShipBackCrackedAimTextureID;

	GLuint  SingleShipTextureID;
	GLuint  SingleShipAimTextureID;
	GLuint  SingleShipCrackedTextureID;
	GLuint  SingleShipCrackedAimTextureID;
	GLuint	SingleShipDeployableTextureID;
	GLuint	SingleShipUndeployableTextureID;

	GLuint  Btn_RotateTextureID;
	GLuint	Btn_RandomAimTextureID;
	GLuint	WaterAimTextureID;

	GLuint	Btn_DownTextureID;
	GLuint	Btn_UpTextureID;
	GLuint	Btn_LeftTextureID;
	GLuint	Btn_RightTextureID;

	GLuint	Btn_FireTextureID;
	GLuint	Btn_DeployTextureID;

	GLuint  Btn_DisconnectTextureID;
	GLuint  Btn_ConnectTextureID;
	GLuint  Btn_CancelTextureID;

	GLuint  Label_NewGame;
	GLuint  Btn_NewGame_PVE;
	GLuint  Btn_NewGame_PVP;

	GLuint	WaterTextureID;
	GLuint	MissedTextureID;
	GLuint	MissedAimTextureID;
};
