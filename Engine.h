#pragma once
#include <Windows.h>

class Engine
{
public:
	Engine();
	void ConvertPixelsToGL(POINT* Pixels);
	void SetMetrics(int Width, int Height);
	void SetOffset();
	float GetOffsetW() { return fOffsetW; }
	float GetOffsetH() { return fOffsetH; }
	void MouseFunc(int X, int Y);
private:
	enum MODE { Connecting, Deploying, MainGame };
	MODE Mode;
	float fOffsetH;
	float fOffsetW;
	int iCurrentHeight;
	int iCurrentWidth;
	int iPixelCellSize;
};
