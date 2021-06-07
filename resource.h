#pragma once

// Bottom level Menus For New Game
#define MENU_GAME_PVP			1

#define MENU_GAME_EASY			11
#define MENU_GAME_MEDIUM		12
#define MENU_GAME_HARD			13

#define MENU_GAME_EXIT			101

//Bottom level Menu For Help
#define MENU_HELP_ABOUT			201

//Messages for MessageParam struct
#define MSG_LBTTNDOWN			1001
#define MSG_RBTTNDOWN			1002

#define MSG_KEYPRESS			2001

//Size of the field array
#define FIELDARRSIZE			2

// Program Metrics
const float MinSizeFactor = 30;

const float OpenGLWidth = 32.0;
const float OpenGLHeight = 16.0;
const float AspectRatio = OpenGLWidth / OpenGLHeight;
const float MinimalWidth = OpenGLWidth * MinSizeFactor;
const float MinimalHeight = OpenGLHeight * (MinSizeFactor + 3.3f);

const int MyGameFieldW = 10;
const int MyGameFieldH = 10;
const int OpponentGameFieldW = 10;
const int OpponentGameFieldH = 10;
