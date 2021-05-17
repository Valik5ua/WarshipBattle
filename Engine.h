#pragma once
#include <Windows.h>

class Engine
{
public:
	Engine();
	bool ConvertPixelsToGL(POINT* Pixels);
	void Mouse();
public:
	float m_fOffestH;
	float m_fOffestW;
	int m_iCurrentHeight;
	int m_iCurrentWidth;
	int m_iPixelCellSize;
private:
	enum MODE { Connecting, Deploying, MainGame };
	MODE Mode;
};
