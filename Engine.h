#pragma once
#include <chrono>
#include "Field.h"
#include "Ship.h"

/// <summary>
/// Game engine.
/// </summary>
class Engine
{
public:
	Engine();
	void ConvertPixelsToGL(POINT* Pixels);
	void SetWindowGLParam(int Width, int Height);

	float GetOffsetW() { return this->fOffsetW; }
	float GetOffsetH() { return this->fOffsetH; }

	bool Event(int MSG, POINT Coordinates = { 0,0 }, unsigned int key = 0);
	void MoveShipToUserField(Ship EnemyFieldShip, Ship& UserFieldShip);
	void Shoot(Field* FieldFrom, Field* FieldTo);

	void IncreaseMatchTime();
	void DecreaseShipsAlive(bool User);

	void StartNewGame();
	void GameOver(bool UserWon);

	bool GetTurn() { return this->UserTurn; }
	void SwitchTurns();

	int GetMatchTime() { return this->MatchTimeSec; }
	int GetPlayerShipsAlive() { return this->PlayerShipsAlive; }
	int GetOpponentShipsAlive() { return this->OpponentShipsAlive; }
public:
	enum GAMEMODE { Menu, PVE, PVP } GameMode;
	enum GAMESTATUS { NewGame, Connecting, Deploying, MainGame } GameStatus;
	enum CONNECTIONMODE { Auto, Manual } ConnectionMode;
	enum ShootStatus { Miss = -1, Damage, KilledOneDeckShip, KilledTwoDeckShip, KilledThreeDeckShip, KilledFourDeckShip } shootStatus;
	enum LastGameResults { N_A, UserWon, OpponentWon } lastGameResults;
	enum Animation { None, Rocket, MainMenu } animation;
	bool LastShotAccomplished;
	int ShipsDeployed;
	float ShootingAngle;
	bool UserShot;
public:
	void SetMode(GAMESTATUS GameStatus);
private:
	bool UserTurn;

	float fOffsetH;
	float fOffsetW;

	float fCurrentHeight;
	float fCurrentWidth;

	float fGLUnitSize;

	unsigned int MatchTimeSec;
	unsigned int PlayerShipsAlive;
	unsigned int OpponentShipsAlive;

	std::chrono::system_clock::duration dtn;
public:
	class AnimationRocket
	{
	private:
		struct FPOINT
		{
			float x;
			float y;
		};
		unsigned int FrameCount;
	public:
		static const unsigned int FramesToDraw = 30;
		FPOINT ShootPoint;
		FPOINT Position[FramesToDraw];
	public:
		AnimationRocket() :FrameCount(0) {}
		void Draw();
	} rocket;

	class MenuAnimation
	{
	private:
		unsigned int FrameCount;
	public:
		//Default direction is to the right
		bool DefaultDirection;
		static const unsigned int FramesToDraw = 30;
		float ClueFieldPositionsX[FramesToDraw];
		float StatusFieldPositionsX[FramesToDraw];
	public:
		MenuAnimation();
		void Draw();
	} menuAnimation;
private:
	struct MessageParam
	{
		POINT FieldCoordinates;
	} MSGParam;

private:
	int TranslateMSG(POINT FieldCoordinates, const int MSG, const unsigned int Key);
	void StartAnimation(Field* field, POINT ShootingPoint);
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

	//Messages when the GAMESTATUS and GAMEMODE are not taken into account
#define TRANSLATEDMSG_SOUNDBUTTONCLICK	14001
};
