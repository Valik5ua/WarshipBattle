#pragma once

//Recources for dialog box and icon loading
#define IDD_DIALOG1				101
#define IDI_ICON1				102
#define IDB_BITMAP1				103

// Bottom level Menus For New Game

#define MENU_GAME_PVP			1
#define MENU_GAME_PVE			2

#define MENU_GAME_EXIT			11

//Bottom level Menu For Help

#define MENU_HELP_ABOUT			101
#define ABOUTBOX				102

//Untranslated Messages For Engine::Event()

#define MSG_LBTTNDOWN			1001
#define MSG_RBTTNDOWN			1002

#define MSG_KEYPRESS			2001

//ButtonField Buttons

#define BF_MOVE_LEFT			14001
#define BF_MOVE_RIGHT			14002
#define BF_MOVE_UP				14003
#define BF_MOVE_DOWN			14004

#define BF_DEPLOY				14101
#define BF_FIRE					14102
#define BF_ROTATE				14103
#define BF_RANDOMAIM			14104

#define BF_NEWGAME_PVE			14201
#define BF_NEWGAME_PVP			14202

#define BF_CONNECT				14111
#define BF_DISCONNECT			14112
#define BF_CANCEL				14113

//TextureID Directions

#define ROTATION_LEFT			20001
#define ROTATION_RIGHT			20002
#define ROTATION_DOWN			20003
#define ROTATION_UP				20004

//Max Ship Count

#define MAX_SHIPS_COUNT			10

//Msg void

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

const unsigned int OpponentGameFieldW = 10;
const unsigned int OpponentGameFieldH = 10;

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
