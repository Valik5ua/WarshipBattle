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

void Engine::SetMetrics(int Width, int Height)
{
	iCurrentHeight = Height;
	iCurrentWidth = Width;
}

void Engine::SetOffset()
{
    if ((float)iCurrentWidth / iCurrentHeight < AspectRatio)
    {
        _Engine.fOffsetW = 0;

        _Engine.iPixelCellSize = _Engine.iCurrentWidth / OpenGLWidth;

        _Engine.fOffsetH = (float)iCurrentHeight - (iCurrentWidth / AspectRatio);
        _Engine.fOffsetH /= (iCurrentWidth / OpenGLWidth * 2);
    }
    else if ((float)iCurrentWidth / iCurrentHeight > AspectRatio)
    {
        _Engine.fOffsetH = 0;

        _Engine.iPixelCellSize = _Engine.iCurrentHeight / OpenGLHeight;

        _Engine.fOffsetW = (float)iCurrentWidth - (iCurrentHeight * AspectRatio);
        _Engine.fOffsetW /= (iCurrentHeight / OpenGLHeight * 2);
    }
}

void Engine::MouseFunc(int X, int Y)
{
    POINT Coordinate{ X,Y };
    ConvertPixelsToGL(&Coordinate);

    static std::ofstream FileMousePos;

    FileMousePos.open("MousePos.test");

    static std::streambuf* sbuf = std::cout.rdbuf();

    std::cout.rdbuf(FileMousePos.rdbuf());
    std::cout << "(" << std::to_string(Coordinate.x) << ",";
    std::cout << std::to_string(Coordinate.y) << ")" << std::endl;
}
