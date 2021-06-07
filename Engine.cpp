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

bool Engine::Event(MessageParam Messageparam, int MSG, unsigned int key)
{

#ifdef TRANSLATE

    translateMSG(MSG, key);
    switch (Mode)
    {
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

#endif //TRANSLATE

    std::string msg = "Field ";
    msg += std::to_string(Messageparam.SelectedFieldArrNum + 1);
    msg += " was clicked! Cell: (";
    msg += std::to_string(Messageparam.GLCoordinates.x);
    msg += ",";
    msg += std::to_string(Messageparam.GLCoordinates.y);
    msg += ")";
    MessageBoxA(hwnd, msg.c_str(), "A field was clicked", NULL);
    return true;
}
