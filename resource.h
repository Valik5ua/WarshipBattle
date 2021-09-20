#pragma once

// Bottom level Menus For New Game

#define MENU_GAME_PVP			1
#define MENU_GAME_PVE			2

#define MENU_GAME_EXIT			11

//Bottom level Menu For Help

#define MENU_HELP_ABOUT			101

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

// Program Metrics

const float MinSizeFactor = 30;

const float OpenGLWidth = 32.0;
const float OpenGLHeight = 16.0;

const float AspectRatio = OpenGLWidth / OpenGLHeight;

const float MinimalWidth = OpenGLWidth * MinSizeFactor;
const float MinimalHeight = OpenGLHeight * (MinSizeFactor + 3.3f);

//Field Metrics

const int UserGameFieldW = 10;
const int UserGameFieldH = 10;

const int OpponentGameFieldW = 10;
const int OpponentGameFieldH = 10;

const int ButtonFieldW = 4;
const int ButtonFieldH = 3;
