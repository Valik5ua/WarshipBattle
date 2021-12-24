#pragma once
#include <Windows.h>

//Positions for all fields
#define UserFieldPosX				3
#define UserFieldPosY				5
#define EnemyFieldPosX				19
#define EnemyFieldPosY				5
#define StatisticFieldPosX			14
#define StatisticFieldPosY			5
#define UserCannonFieldPosX			1
#define UserCannonFieldPosY			9
#define EnemyCannonFieldPosX		29
#define EnemyCannonFieldPosY		9
#define ButtonFieldFirePosX			25
#define ButtonFieldFirePosY			1
#define ButtonFieldDeployPosX		3
#define ButtonFieldDeployPosY		1
#define ButtonFieldNewGamePosX		3
#define ButtonFieldNewGamePosY		1
#define ButtonFieldConnectPosX		3
#define ButtonFieldConnectPosY		1
#define ClueFieldPosX				13
#define ClueFieldPosY				1
#define StatusFieldPosX				8
#define StatusFieldPosY				1
#define ClueFieldMainGamePosX		3
#define ClueFieldMainGamePosY		1
#define StatusFieldMainGamePosX		19
#define StatusFieldMainGamePosY		1
#define SoundButtonPosX				14
#define SoundButtonPosY				15

//Defines for icon, and dialogbox.
#define IDD_DIALOG1					101
#define IDD_DIALOG3					102
#define IDI_ICON1					103
#define IDB_BITMAP1					104

//Defines for sounds loaded into Warship Battle.exe
#define S_WAVE_CLICK				1001
#define S_WAVE_ERROR				1002
#define S_WAVE_DAMAGE				1003
#define S_WAVE_DEPLOY				1004
#define S_WAVE_KILL					1005
#define S_WAVE_LOSE					1006
#define S_WAVE_WIN					1007
#define S_WAVE_ROTATE				1008
#define S_WAVE_SHOOT				1009

// Bottom level Menus For New Game

#define MENU_GAME_MAINMENU			1
#define MENU_GAME_PVP				2
#define MENU_GAME_PVE				3

#define MENU_GAME_EXIT				11

//Bottom level Menu For Help

#define MENU_HELP_ABOUT				101
#define ABOUTBOX					102

//Untranslated Messages For Engine::Event()

#define MSG_LBTTNDOWN				1001
#define MSG_RBTTNDOWN				1002

#define MSG_KEYPRESS				2001

//ButtonField Buttons

#define BF_MOVE_LEFT				14001
#define BF_MOVE_RIGHT				14002
#define BF_MOVE_UP					14003
#define BF_MOVE_DOWN				14004

#define BF_DEPLOY					14101
#define BF_FIRE						14102
#define BF_ROTATE					14103
#define BF_RANDOMAIM				14104

#define BF_NEWGAME_PVE				14201
#define BF_NEWGAME_PVP				14202

#define BF_CONNECT_BOTTOM_BUTTON	14111
#define BF_CONNECT_MIDDLE_BUTTON	14112
#define BF_CONNECT_TOP_BUTTON		14113

//TextureID Directions

#define ROTATION_LEFT				20001
#define ROTATION_RIGHT				20002
#define ROTATION_DOWN				20003
#define ROTATION_UP					20004

//Max Ship Count

#define MAX_SHIPS_COUNT				10

//Msg Void

#define MSG_VOID					-1

// Program Metrics

const float OpenGLWidth = 32.0;
const float OpenGLHeight = 16.0;

const float AspectRatio = OpenGLWidth / OpenGLHeight;

const float MinimalHeight = 625;
const float MinimalWidth = MinimalHeight * 2;

//Field Metrics

const unsigned int UserGameFieldW = 10;
const unsigned int UserGameFieldH = 10;

const unsigned int EnemyGameFieldW = 10;
const unsigned int EnemyGameFieldH = 10;

const unsigned int StatisticFieldW = 4;
const unsigned int StatisticFieldH = 10;

const unsigned int ButtonFieldW = 4;
const unsigned int ButtonFieldH = 3;

const unsigned int ClueFieldW = 16;
const unsigned int ClueFieldH = 3;

const unsigned int StatusFieldW = 5;
const unsigned int StatusFieldH = 3;

const unsigned int CannonFieldW = 2;
const unsigned int CannonFieldH = 2;

const unsigned int SoundButtonW = 4;
const unsigned int SoundButtonH = 1;
