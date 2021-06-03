#pragma once
#include <Windows.h>
#include "Field.h"

class Engine
{
public:
	Engine();
	void ConvertPixelsToGL(POINT* Pixels);
	void SetWindowGLParam(int Width, int Height);
	float GetOffsetW() { return fOffsetW; }
	float GetOffsetH() { return fOffsetH; }
	bool ClickEvent(Field* ClickedField, POINT ClickCoordinates);
private:
	enum MODE { Connecting, Deploying, MainGame };
	MODE Mode;
	float fOffsetH;
	float fOffsetW;
	float fCurrentHeight;
	float fCurrentWidth;
	float fGLUnitSize;
};
