#include <fstream>
#include <iostream>
#include <string>
#include "resource.h"

Engine::Engine() :Mode(Connecting), fOffsetH(0), fOffsetW(0), iCurrentHeight(0), iCurrentWidth(0), iPixelCellSize(0)
{

}

void Engine::ConvertPixelsToGL(POINT* Pixels)
{
    Pixels->y = OpenGLHeight * iPixelCellSize - Pixels->y;
	Pixels->x /= iPixelCellSize;
	Pixels->y /= iPixelCellSize;
    Pixels->y += fOffsetH;
    Pixels->x -= fOffsetW;
}

void Engine::SetWindowGLParam(int Width, int Height)
{
    iCurrentHeight = Height;
    iCurrentWidth = Width;
        _Engine.fOffsetW = 0;
        _Engine.fOffsetH = 0;
    if ((float)iCurrentWidth / iCurrentHeight < AspectRatio)
    {

        _Engine.iPixelCellSize = _Engine.iCurrentWidth / OpenGLWidth;

        _Engine.fOffsetH = (float)iCurrentHeight - (iCurrentWidth / AspectRatio);
        _Engine.fOffsetH /= (iCurrentWidth / OpenGLWidth * 2);
    }
    else if ((float)iCurrentWidth / iCurrentHeight > AspectRatio)
    {

        _Engine.iPixelCellSize = _Engine.iCurrentHeight / OpenGLHeight;

        _Engine.fOffsetW = (float)iCurrentWidth - (iCurrentHeight * AspectRatio);
        _Engine.fOffsetW /= (iCurrentHeight / OpenGLHeight * 2);
    }
}
