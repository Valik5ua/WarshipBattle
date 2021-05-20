#include "resource.h"

Engine::Engine() :Mode(Connecting), fOffsetH(0), fOffsetW(0), iCurrentHeight(0), iCurrentWidth(0), fPixelCellSize(0)
{

}

void Engine::ConvertPixelsToGL(POINT* Pixels)
{
    Pixels->y = OpenGLHeight * fPixelCellSize - Pixels->y;
    Pixels->y += fOffsetH * fPixelCellSize;
    Pixels->x -= fOffsetW * fPixelCellSize;
	Pixels->x /= fPixelCellSize;
	Pixels->y /= fPixelCellSize;
}

void Engine::SetWindowGLParam(int Width, int Height)
{
    iCurrentHeight = Height;
    iCurrentWidth = Width;
    _Engine.fOffsetW = 0;
    _Engine.fOffsetH = 0;
    if ((float)iCurrentWidth / iCurrentHeight < AspectRatio)
    {

        _Engine.fPixelCellSize = _Engine.iCurrentWidth / OpenGLWidth;

        _Engine.fOffsetH = (float)iCurrentHeight - (iCurrentWidth / AspectRatio);
        _Engine.fOffsetH /= (iCurrentWidth / OpenGLWidth * 2);
    }
    else if ((float)iCurrentWidth / iCurrentHeight > AspectRatio)
    {

        _Engine.fPixelCellSize = (float)_Engine.iCurrentHeight / OpenGLHeight;

        _Engine.fOffsetW = (float)iCurrentWidth - (iCurrentHeight * AspectRatio);
        _Engine.fOffsetW /= (iCurrentHeight / OpenGLHeight * 2);
    }
}
