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
	GLuint	ShipAfireTextureID;
	GLuint	ShipAfireAimTextureID;

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

	GLuint	ButtonFieldFireTextureID;
	GLuint	ButtonFieldDeployTextureID;

	GLuint	ButtonFieldConnect_AutoManualTextureID;
	GLuint	ButtonFieldConnect_A_ConnectionTextureID;
	GLuint	ButtonFieldConnect_M_ServerClientTextureID;
	GLuint	ButtonFieldConnect_M_ClientConnectionTextureID;
	GLuint	ButtonFieldConnect_M_ServerConnectionTextureID;

	GLuint	ClueFieldNewGameTextureID;
	GLuint	ClueFieldDeployingTextureID;
	GLuint	ClueFieldMainGameTextureID;
	GLuint	ClueFieldYouWonTextureID;
	GLuint	ClueFieldYouLostTextureID;

	GLuint	StatusFieldTopPVETextureID;
	GLuint	StatusFieldTopMainMenuTextureID;

	GLuint	StatusFieldBottomYourTurnTextureID;
	GLuint	StatusFieldBottomChooseGameTextureID;
	GLuint	StatusFieldBottomDeployShipsTextureID;
	GLuint	StatusFieldBottomOpponentTurnTextureID;

	GLuint	Btn_FireTextureID;
	GLuint	Btn_DeployTextureID;

	GLuint  Btn_DisconnectTextureID;
	GLuint  Btn_ConnectTextureID;
	GLuint  Btn_CancelTextureID;

	GLuint  Label_NewGame;
	GLuint  Btn_NewGame_PVE;
	GLuint  Btn_NewGame_PVP;

	GLuint  StatisticFieldTextureID;

	GLuint	WaterTextureID;
	GLuint	MissedTextureID;
	GLuint	MissedAimTextureID;

	GLuint  TimeNumber0TextureID;
	GLuint  TimeNumber1TextureID;
	GLuint  TimeNumber2TextureID;
	GLuint  TimeNumber3TextureID;
	GLuint  TimeNumber4TextureID;
	GLuint  TimeNumber5TextureID;
	GLuint  TimeNumber6TextureID;
	GLuint  TimeNumber7TextureID;
	GLuint  TimeNumber8TextureID;
	GLuint  TimeNumber9TextureID;
	GLuint  TimeColonTextureID;

	GLuint  AliveDeadNumber0TextureID;
	GLuint  AliveDeadNumber1TextureID;
	GLuint  AliveDeadNumber2TextureID;
	GLuint  AliveDeadNumber3TextureID;
	GLuint  AliveDeadNumber4TextureID;
	GLuint  AliveDeadNumber5TextureID;
	GLuint  AliveDeadNumber6TextureID;
	GLuint  AliveDeadNumber7TextureID;
	GLuint  AliveDeadNumber8TextureID;
	GLuint  AliveDeadNumber9TextureID;
	GLuint  AliveDeadNumber10TextureID;

	GLuint CannonBackroundTextureID;
	GLuint CannonTextureID;

	GLuint ExplosionTextureID;

	GLuint RocketBodyTextureID;
	GLuint RocketTopTextureID;
	GLuint RocketFinTopTextureID;
	GLuint RocketFinBottomTextureID;

	GLuint SoundOnTextureID;
	GLuint SoundOffTextureID;
};
