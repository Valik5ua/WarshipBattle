#pragma once
#include "Field.h"
#include "Ship.h"

class Engine
{
public:
	Engine();
	void ConvertPixelsToGL(POINT* Pixels);
	void SetWindowGLParam(int Width, int Height);
	float GetOffsetW() { return fOffsetW; }
	float GetOffsetH() { return fOffsetH; }
	bool Event(int MSG, POINT Coordinates = { 0,0 }, unsigned int key = 0);
	void MoveShipToUserField(Ship EnemyFieldShip, Ship& UserFieldShip);
	bool GetTurn() { return this->UserTurn; }
	void Shoot(Field* FieldFrom, Field* FieldTo);

public:
	enum GAMEMODE { Menu, PVE, PVP } GameMode;
	enum GAMESTATUS { NewGame, Connecting, Deploying, MainGame } GameStatus;
	enum CONNECTIONMODE { Auto, Manual } ConnectionMode;
	enum ShootStatus { Miss = -1, Damage, KilledOneDeckShip, KilledTwoDeckShip, KilledThreeDeckShip, KilledFourDeckShip	} shootStatus;
	int ShipsDeployed;
public:
	void SetMode(GAMESTATUS GameStatus);
private:
	bool UserTurn;

	float fOffsetH;
	float fOffsetW;

	float fCurrentHeight;
	float fCurrentWidth;

	float fGLUnitSize;

private:
	struct MessageParam
	{
		POINT FieldCoordinates;
	} MSGParam;

private:
	int TranslateMSG(POINT FieldCoordinates, const int MSG, const unsigned int Key);
	void SwitchTurns();
private:
	//Fully translated messages for Engine::Event


	//Messages when GAMESTATUS::Connecting is the current mode
#define TRANSLATEDMSG_CONNECT			10001
#define TRANSLATEDMSG_DISCONNECT		10002

	//Messages when GAMESTATUS::Deploying is the current mode
#define TRANSLATEDMSG_SELECTSHIP		11001
#define TRANSLATEDMSG_MOVESHIPL			11002
#define TRANSLATEDMSG_MOVESHIPR			11003
#define TRANSLATEDMSG_MOVESHIPUP		11004
#define TRANSLATEDMSG_MOVESHIPDOWN		11005

#define TRANSLATEDMSG_DEPLOY			11101
#define TRANSLATEDMSG_ROTATE			11102

	//Messages when GAMESTATUS::MainGame is the current mode
#define TRANSLATEDMSG_AIM				12001
#define TRANSLATEDMSG_FIRE				12002
#define	TRANSLATEDMSG_RANDOMAIM			12003

#define TRANSLATEDMSG_MOVE_LEFT			12101
#define TRANSLATEDMSG_MOVE_RIGHT		12102
#define TRANSLATEDMSG_MOVE_UP			12103
#define TRANSLATEDMSG_MOVE_DOWN			12104

	//Messages when GAMESTATUS::NewGame is the current GAMESTATUS
#define TRANSLATEDMSG_NEWGAMEPVE		13001
#define TRANSLATEDMSG_NEWGAMEPVP		13002
};