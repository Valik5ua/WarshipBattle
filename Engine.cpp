#include "Engine.h"
#include <math.h>
#include <string>
#include "UserField.h"
#include "EnemyField.h"
#include "ButtonFieldDeploy.h"
#include "ButtonFieldFire.h"
#include "ButtonFieldConnect.h"

extern const float OpenGLHeight;
extern const float OpenGLWidth;
extern const float AspectRatio;

extern Engine::MODE ButtonFieldMode;
extern UserField userField;
extern EnemyField enemyField;
extern ButtonFieldDeploy buttonFieldDeploy;
extern ButtonFieldFire buttonFieldFire;
extern ButtonFieldConnect buttonFieldConnect;

Engine::Engine() :Mode(Deploying), fOffsetH(0), fOffsetW(0), fCurrentHeight(0), fCurrentWidth(0), fGLUnitSize(0)
{
    ButtonFieldMode = this->Mode;
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
        case TRANSLATEDMSG_DEPLOY:
            break;
        case TRANSLATEDMSG_ROTATE:
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
            enemyField.Select(MSGParam.FieldCoordinates.x, MSGParam.FieldCoordinates.y);
            break;
        case TRANSLATEDMSG_MOVE_LEFT:
            enemyField.MoveSelection(BF_MOVE_LEFT);
            break;
        case TRANSLATEDMSG_MOVE_RIGHT:
            enemyField.MoveSelection(BF_MOVE_RIGHT);
            break;
        case TRANSLATEDMSG_MOVE_DOWN:
            enemyField.MoveSelection(BF_MOVE_DOWN);
            break;
        case TRANSLATEDMSG_MOVE_UP:
            enemyField.MoveSelection(BF_MOVE_UP);
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
    switch (Mode)
    {
    case MODE::Deploying:
    {
        ButtonFieldMode = MODE::Deploying;
        if (MSG == MSG_LBTTNDOWN)
        {
            if (buttonFieldDeploy.Click(Coordinates))
            {
                MSGParam.FieldCoordinates = Coordinates;
                switch (buttonFieldDeploy.Cells[Coordinates.x][Coordinates.y].ButtonID)
                {
                case BF_MOVE_DOWN:
                    return TRANSLATEDMSG_MOVESHIPDOWN;
                case BF_MOVE_LEFT:
                    return TRANSLATEDMSG_MOVESHIPL;
                case BF_MOVE_UP:
                    return TRANSLATEDMSG_MOVESHIPUP;
                case BF_MOVE_RIGHT:
                    return TRANSLATEDMSG_MOVESHIPR;
                default: return MSG_VOID;
                }
            }
            if (MSG == MSG_KEYPRESS)
            {
                switch (Key)
                {
                case VK_DOWN:
                    return TRANSLATEDMSG_MOVE_DOWN;
                case VK_LEFT:
                    return TRANSLATEDMSG_MOVE_LEFT;
                case VK_UP:
                    return TRANSLATEDMSG_MOVE_UP;
                case VK_RIGHT:
                    return TRANSLATEDMSG_MOVE_RIGHT;
                case 13:
                    return TRANSLATEDMSG_DEPLOY;
                case 32:
                    return TRANSLATEDMSG_ROTATE;
                default: return MSG_VOID;
                }
            }
        }
    }
    case MODE::MainGame:
    {
        ButtonFieldMode = MODE::MainGame;
        if (MSG == MSG_LBTTNDOWN)
        {
            if (enemyField.Click(Coordinates))
            {
                MSGParam.FieldCoordinates = Coordinates;
                return TRANSLATEDMSG_AIM;
            }
            if (buttonFieldDeploy.Click(Coordinates))
            {
                MSGParam.FieldCoordinates = Coordinates;
                switch (buttonFieldDeploy.Cells[Coordinates.x][Coordinates.y].ButtonID)
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
                case BF_RANDOMAIM:
                    return TRANSLATEDMSG_RANDOMAIM;
                default: return MSG_VOID;
                }
            }
        }
        if (MSG == MSG_KEYPRESS)
        {
            switch (Key)
            {
            case VK_DOWN:
                return TRANSLATEDMSG_MOVE_DOWN;
            case VK_LEFT:
                return TRANSLATEDMSG_MOVE_LEFT;
            case VK_UP:
                return TRANSLATEDMSG_MOVE_UP;
            case VK_RIGHT:
                return TRANSLATEDMSG_MOVE_RIGHT;
            case 13:
                return TRANSLATEDMSG_FIRE;
            case 32:
                return TRANSLATEDMSG_RANDOMAIM;
            default: return MSG_VOID;
            }
        }
        return MSG_VOID;
    }
    case MODE::Connecting:
    {
        ButtonFieldMode = MODE::Connecting;
    }
    }
}
