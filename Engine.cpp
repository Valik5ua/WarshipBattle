#include "Engine.h"
#include <math.h>
#include <string>
#include "UserField.h"
#include "EnemyField.h"
#include "ButtonField.h"

extern const float OpenGLHeight;
extern const float OpenGLWidth;
extern const float AspectRatio;
extern UserField _UserField;
extern EnemyField _EnemyField;
extern ButtonField _ButtonField;

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
            _UserField.Select(MSGParam.FieldCoordinates.x,MSGParam.FieldCoordinates.y);
        }
        break;
        case TRANSLATEDMSG_ENEMYFIELDCLICK:
        {
            _EnemyField.Select(MSGParam.FieldCoordinates.x,MSGParam.FieldCoordinates.y);
        }
        break;
        case TRANSLATEDMSG_MOVE_LEFT:
            _EnemyField.MoveSelection(BF_MOVE_LEFT);
            break;
        case TRANSLATEDMSG_MOVE_RIGHT:
            _EnemyField.MoveSelection(BF_MOVE_RIGHT);
            break;
        case TRANSLATEDMSG_MOVE_DOWN:
            _EnemyField.MoveSelection(BF_MOVE_DOWN);
            break;
        case TRANSLATEDMSG_MOVE_UP:
            _EnemyField.MoveSelection(BF_MOVE_UP);
            break;
        case TRANSLATEDMSG_DEPLOY:
            break;
        case TRANSLATEDMSG_ROTATE:
            break;
        default: break;
        }
    }
    break;

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
        case TRANSLATEDMSG_MOVE_LEFT:
            _EnemyField.MoveSelection(BF_MOVE_LEFT);
            break;
        case TRANSLATEDMSG_MOVE_RIGHT:
            _EnemyField.MoveSelection(BF_MOVE_RIGHT);
            break;
        case TRANSLATEDMSG_MOVE_DOWN:
            _EnemyField.MoveSelection(BF_MOVE_DOWN);
            break;
        case TRANSLATEDMSG_MOVE_UP:
            _EnemyField.MoveSelection(BF_MOVE_UP);
            break;
        case TRANSLATEDMSG_FIRE:

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
        if (_ButtonField.Click(Coordinates))
        {
            MSGParam.FieldCoordinates = Coordinates;
            switch (_ButtonField.Cells[Coordinates.x][Coordinates.y].ButtonID)
            {
            case BF_MOVE_DOWN:
                return TRANSLATEDMSG_MOVE_DOWN;
            case BF_MOVE_LEFT:
                return TRANSLATEDMSG_MOVE_LEFT;
            case BF_MOVE_UP:
                return TRANSLATEDMSG_MOVE_UP;
            case BF_MOVE_RIGHT:
                return TRANSLATEDMSG_MOVE_RIGHT;
            case BF_FIRE:
                return TRANSLATEDMSG_FIRE;
            case BF_ROTATE:
                return TRANSLATEDMSG_ROTATE;
            default: return MSG_VOID;
            }
        }
    }
    if (MSG == MSG_KEYPRESS)
    {
        switch (Key)
        {
        case BF_MOVE_DOWN:
            return TRANSLATEDMSG_MOVE_DOWN;
        case BF_MOVE_LEFT:
            return TRANSLATEDMSG_MOVE_LEFT;
        case BF_MOVE_UP:
            return TRANSLATEDMSG_MOVE_UP;
        case BF_MOVE_RIGHT:
            return TRANSLATEDMSG_MOVE_RIGHT;
        case BF_FIRE:
            return TRANSLATEDMSG_DEPLOY;
        case BF_ROTATE:
            return TRANSLATEDMSG_ROTATE;
        default: return MSG_VOID;
        }
    }
    return MSG_VOID;
}
