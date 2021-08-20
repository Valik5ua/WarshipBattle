#pragma once

// Bottom level Menus For New Game
#define MENU_GAME_PVP			1

#define MENU_GAME_EASY			11
#define MENU_GAME_MEDIUM		12
#define MENU_GAME_HARD			13

#define MENU_GAME_EXIT			21

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

// Program Metrics
const float MinSizeFactor = 30;

const float OpenGLWidth = 32.0;
const float OpenGLHeight = 16.0;

const float AspectRatio = OpenGLWidth / OpenGLHeight;

const float MinimalWidth = OpenGLWidth * MinSizeFactor;
const float MinimalHeight = OpenGLHeight * (MinSizeFactor + 3.3f);

//Field Metrics
const int MyGameFieldW = 10;
const int MyGameFieldH = 10;

const int OpponentGameFieldW = 10;
const int OpponentGameFieldH = 10;

const int ButtonFieldW = 4;
const int ButtonFieldH = 3;
