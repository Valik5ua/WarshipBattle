#include "Engine.h"
#include <math.h>
#include <string>
#include "UserField.h"
#include "EnemyField.h"

extern const float OpenGLHeight;
extern const float OpenGLWidth;
extern const float AspectRatio;
extern UserField _UserField;
extern EnemyField _EnemyField;

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

bool Engine::Event(int MSG, POINT Coordinates, unsigned int key)
{
    int TranslatedMSG = TranslateMSG(Coordinates, MSG, key);
    switch (Mode)
    {
#ifdef DEBUG1

    case MODE::WaitingForAction:
    {
        switch (TranslatedMSG)
        {
        case TRANSLATEDMSG_USERFIELDCLICK:
        {
            _UserField.Cells[MSGParam.FieldCoordinates.x][MSGParam.FieldCoordinates.y].Stat = Cell::Status::opened;
            _UserField.Select(MSGParam.FieldCoordinates.x,MSGParam.FieldCoordinates.y);
        }
        break;
        case TRANSLATEDMSG_ENEMYFIELDCLICK:
        {
            _EnemyField.Select(MSGParam.FieldCoordinates.x,MSGParam.FieldCoordinates.y);
            _EnemyField.Cells[MSGParam.FieldCoordinates.x][MSGParam.FieldCoordinates.y].Stat = Cell::Status::opened;
        }
        break;
        default: break;
        }
    }

#endif //DEBUG1

    case MODE::Connecting:
    {
        switch (TranslatedMSG)
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
        switch (TranslatedMSG)
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
        switch (TranslatedMSG)
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

int Engine::TranslateMSG(POINT Coordinates, const int MSG, const unsigned int Key)
{
    if (MSG == MSG_LBTTNDOWN)
    {
        if (_UserField.Click(Coordinates))
        {
            MSGParam.FieldCoordinates = Coordinates;
            return TRANSLATEDMSG_USERFIELDCLICK;
        }
        if (_EnemyField.Click(Coordinates))
        {
            MSGParam.FieldCoordinates = Coordinates;
            return TRANSLATEDMSG_ENEMYFIELDCLICK;
        }
    }
}
