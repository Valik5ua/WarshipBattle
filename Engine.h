#pragma once
#include "Connection.h"
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

	/// <summary>
	/// A function that returns whose turn it is to shoot.
	/// </summary>
	/// <returns>Wether or not the current turn is the user's.</returns>
	bool GetTurn() { return this->UserTurn; }

	void SwitchTurns();

	int GetMatchTime() { return this->MatchTimeSec; }
	int GetPlayerShipsAlive() { return this->PlayerShipsAlive; }
	int GetOpponentShipsAlive() { return this->OpponentShipsAlive; }
public:

	/// <summary>
	/// An enumerator dedicated to keeping the game's current mode.
	/// </summary>
	enum GAMEMODE { Menu, PVE, PVP } GameMode;

	enum GAMESTATUS {	NewGame,
						Connecting,
						Deploying,
						MainGame,
						ChoosingConnectionMode,
						ChoosingConnectionSide,
						ServerConnection,
						ClientConnection,
						AutoConnection,
						Disconnecting
					} GameStatus;

	enum ShootStatus {	Miss = -1,
						Damage,
						KilledOneDeckShip,
						KilledTwoDeckShip,
						KilledThreeDeckShip,
						KilledFourDeckShip 
					} shootStatus;
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

	Connection* connection;
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

	//Messages when GAMESTATUS::Deploying is the current mode
#define TRANSLATEDMSG_SELECTSHIP			10001
#define TRANSLATEDMSG_MOVESHIPL				10002
#define TRANSLATEDMSG_MOVESHIPR				10003
#define TRANSLATEDMSG_MOVESHIPUP			10004
#define TRANSLATEDMSG_MOVESHIPDOWN			10005

#define TRANSLATEDMSG_DEPLOY				10101
#define TRANSLATEDMSG_ROTATE				10102

	//Messages when GAMESTATUS::MainGame is the current mode
#define TRANSLATEDMSG_AIM					11001
#define TRANSLATEDMSG_FIRE					11002
#define	TRANSLATEDMSG_RANDOMAIM				11003

#define TRANSLATEDMSG_MOVE_LEFT				11101
#define TRANSLATEDMSG_MOVE_RIGHT			11102
#define TRANSLATEDMSG_MOVE_UP				11103
#define TRANSLATEDMSG_MOVE_DOWN				11104

	//Messages when GAMESTATUS::NewGame is the current GAMESTATUS
#define TRANSLATEDMSG_NEWGAMEPVE			12001
#define TRANSLATEDMSG_NEWGAMEPVP			12002

	//Messages when Connection is being picked/when connecting is happening
#define	TRANSLATEDMSG_CONNECTION_CANCEL		13000
#define	TRANSLATEDMSG_CONNECTION_AUTO		13001
#define	TRANSLATEDMSG_CONNECTION_MANUAL		13002
#define	TRANSLATEDMSG_CONNECTION_SERVER		13003
#define	TRANSLATEDMSG_CONNECTION_CLIENT		13004
#define	TRANSLATEDMSG_CONNECTION_INPUTIP	13011
#define	TRANSLATEDMSG_CONNECTION_SHOWIP		13012

	//Messages when the GAMESTATUS and GAMEMODE are not taken into account
#define TRANSLATEDMSG_SOUNDBUTTONCLICK		15001
};
