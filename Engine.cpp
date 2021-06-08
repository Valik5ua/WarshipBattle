#include "Engine.h"
#include <math.h>
#include <string>

extern const float OpenGLHeight;
extern const float OpenGLWidth;
extern const float AspectRatio;
extern Field* FieldArr[2];

Engine::Engine() :Mode(WaitingForAction), fOffsetH(0), fOffsetW(0), fCurrentHeight(0), fCurrentWidth(0), fGLUnitSize(0)
{
}

/// <summary>
/// Converts pixels to OpenGL units.
/// </summary>
/// <param name="Pixels: ">pointer to the desired POINT struct to convert.</param>
void Engine::ConvertPixelsToGL(POINT* Pixels)
{
    Pixels->y = OpenGLHeight * fGLUnitSize - Pixels->y;
    Pixels->y = floor((Pixels->y / fGLUnitSize) + fOffsetH);
    Pixels->x = floor((Pixels->x / fGLUnitSize) - fOffsetW);
}

/// <summary>
/// Sets all the required OpenGL context parameters.
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
        fGLUnitSize = fCurrentWidth / OpenGLWidth;
        fOffsetH = (fCurrentHeight / fGLUnitSize - OpenGLHeight) / 2;
    }
    else if (fCurrentWidth / fCurrentHeight > AspectRatio)
    {
        fGLUnitSize = fCurrentHeight / OpenGLHeight;
        fOffsetW = (fCurrentWidth / fGLUnitSize - OpenGLWidth) / 2;
    }
}

bool Engine::Event(POINT Coordinates, int MSG, unsigned int key)
{
    TranslateMSG(Coordinates, MSG, key);
    switch (Mode)
    {
#ifdef TRANSLATE

    case MODE::WaitingForAction:
    {
        switch (MSG)
        {
        case TRANSLATEDMSG_USERFIELDCLICK:
        {
            std::string msg = "User Field was clicked! Cell: (";
            msg += std::to_string(MSGParam.GLCoordinates.x);
            msg += ",";
            msg += std::to_string(MSGParam.GLCoordinates.y);
            msg += ")";
            MessageBoxA(hwnd, msg.c_str(), "User field was clicked", NULL);
        }
        break;
        case TRANSLATEDMSG_ENEMYFIELDCLICK:
        {
            std::string msg = "Enemy Field was clicked! Cell: (";
            msg += std::to_string(MSGParam.GLCoordinates.x);
            msg += ",";
            msg += std::to_string(MSGParam.GLCoordinates.y);
            msg += ")";
            MessageBoxA(hwnd, msg.c_str(), "Enemy field was clicked", NULL);
        }
        break;
        default: break;
        }
    }

#endif //TRANSLATE

    case MODE::Connecting:
    {
        switch (MSG)
        {
        case TRANSLATEDMSG_CONNECT:
        {
        }
        break;
        case TRANSLATEDMSG_DISCONNECT:
        {
        }
        break;
        default: 
            break;
        }
    }
    break;
    case MODE::Deploying:
    {
        switch (MSG)
        {
        case TRANSLATEDMSG_SELECTSHIP:
        {
        }
        break;
        case TRANSLATEDMSG_MOVESHIPL:
        {
        }
        break;
        case TRANSLATEDMSG_MOVESHIPR:
        {
        }
        break;
        case TRANSLATEDMSG_MOVESHIPUP:
        {
        }
        break;
        case TRANSLATEDMSG_MOVESHIPDOWN:
        {
        }
        break;
        default:
            break;
        }
    }
    break;
    case MODE::MainGame:
    {
        switch (MSG)
        {
        case TRANSLATEDMSG_AIM:
        {
        }
        break;
        case TRANSLATEDMSG_FIRE:
        {
        }
        break;
        default:
            break;
        }
}
    break;
    default:
        return false;
    }
    return true;
}
bool Engine::TranslateMSG(POINT GLCoordinates, int& MSG, unsigned int Key)
{
    size_t ActiveArray = 0;
    for (; ActiveArray < FIELDARRSIZE; ActiveArray++)
    {
        if (FieldArr[ActiveArray]->Click(GLCoordinates)) break;
        if (ActiveArray == FIELDARRSIZE - 1) return false;
    }
    MSGParam.GLCoordinates = GLCoordinates;
    if (ActiveArray == 0) MSG = TRANSLATEDMSG_USERFIELDCLICK;
    if (ActiveArray == 1) MSG = TRANSLATEDMSG_ENEMYFIELDCLICK;
    return true;
}
