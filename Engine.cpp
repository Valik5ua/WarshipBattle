#include "resource.h"
#include <math.h>

Engine::Engine() :Mode(Connecting), fOffsetH(0), fOffsetW(0), fCurrentHeight(0), fCurrentWidth(0), fPixelCellSize(0)
{

}

/// <summary>
/// Converts pixels to OpenGL units.
/// </summary>
/// <param name="Pixels: ">pointer to the desired POINT struct to convert.</param>
void Engine::ConvertPixelsToGL(POINT* Pixels)
{
    Pixels->y = OpenGLHeight * fPixelCellSize - Pixels->y;
    Pixels->y = floor((Pixels->y / fPixelCellSize) + fOffsetH);
    Pixels->x = floor((Pixels->x / fPixelCellSize) - fOffsetW);
}

/// <summary>
/// Sets all the required OpenGL related parameters.
/// </summary>
/// <param name="Width: ">The current width of the window.</param>
/// <param name="Height: ">The current height of the window.</param>
void Engine::SetWindowGLParam(int Width, int Height)
{
    fCurrentHeight = Height;
    fCurrentWidth = Width;
    fOffsetW = 0;
    fOffsetH = 0;
    if (fCurrentWidth / fCurrentHeight < AspectRatio)
    {
        fPixelCellSize = fCurrentWidth / OpenGLWidth;
        fOffsetH = (fCurrentHeight / fPixelCellSize - OpenGLHeight) / 2;
    }
    else if (fCurrentWidth / fCurrentHeight > AspectRatio)
    {
        fPixelCellSize = fCurrentHeight / OpenGLHeight;
        fOffsetW = (fCurrentWidth / fPixelCellSize - OpenGLWidth) / 2;
    }
}
