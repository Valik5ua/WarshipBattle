#pragma once
#include <Windows.h>
#include "Field.h"

#define TRANSLATE

class Engine
{
public:
	Engine();
	void ConvertPixelsToGL(POINT* Pixels);
	void SetWindowGLParam(int Width, int Height);
	float GetOffsetW() { return fOffsetW; }
	float GetOffsetH() { return fOffsetH; }
	bool Event(int MSG, POINT Coordinates = { 0,0 }, unsigned int key = 0);
private:
	enum MODE { WaitingForAction, Connecting, Deploying, MainGame, ComputerTurn };
	MODE Mode;

	float fOffsetH;
	float fOffsetW;

	float fCurrentHeight;
	float fCurrentWidth;

	float fGLUnitSize;

private:

	struct MessageParam
	{
		//POINT InputCoordinates;
		POINT FieldCoordinates;
	} MSGParam;

private: 
	int TranslateMSG(POINT FieldCoordinates, const int MSG, const unsigned int Key);
private:
	//Fully translated messages for Engine::Event
	
	//Messages when MODE::Connecting is the current mode
#define TRANSLATEDMSG_CONNECT		10001
#define TRANSLATEDMSG_DISCONNECT    10002

	//Messages when MODE::Deploying is the current mode
#define TRANSLATEDMSG_SELECTSHIP    11001
#define TRANSLATEDMSG_MOVESHIPL	    11002
#define TRANSLATEDMSG_MOVESHIPR	    11003
#define TRANSLATEDMSG_MOVESHIPUP    11004
#define TRANSLATEDMSG_MOVESHIPDOWN  11005

	//Messages when MODE::MainGame is the current mode
#define TRANSLATEDMSG_AIM		    12001
#define TRANSLATEDMSG_FIRE		    12002

#define TRANSLATEDMSG_USERFIELDCLICK	13001
#define TRANSLATEDMSG_ENEMYFIELDCLICK	13002
};
