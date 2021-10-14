#include "Engine.h"
#include <math.h>
#include <string>
#include "UserField.h"
#include "EnemyField.h"
#include "ButtonFieldDeploy.h"
#include "ButtonFieldFire.h"
#include "ButtonFieldConnect.h"
#include "ButtonFieldNewGame.h"

extern const float OpenGLHeight;
extern const float OpenGLWidth;
extern const float AspectRatio;

extern UserField userField;
extern EnemyField enemyField;
extern ButtonFieldDeploy buttonFieldDeploy;
extern ButtonFieldFire buttonFieldFire;
extern ButtonFieldConnect buttonFieldConnect;
extern ButtonFieldNewGame buttonFieldNewGame;

// define while only PVE game is avaliable
#define GAMEMODE_PVE_ONLY

/// <summary>
/// Default constructor for engine class.
/// </summary>
Engine::Engine() :GameStatus(NewGame), fOffsetH(0), fOffsetW(0), fCurrentHeight(0), fCurrentWidth(0), fGLUnitSize(0), ShipsDeployed(0), UserTurn(true)
{
#ifdef GAMEMODE_PVE_ONLY
    this->GameMode = this->GAMEMODE::Menu;
#endif // GAMEMODE_PVE_ONLY
}

/// <summary>
/// Converts pixels to OpenGL units.
/// </summary>
/// <param name="Pixels: ">Pointer to the desired POINT struct to convert.</param>
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

/// <summary>
/// This function takes an input, translates it, and then does something dependent on the translation.
/// </summary>
/// <param name="MSG: ">The untranslated message.</param>
/// <param name="Coordinates: ">The coordinates of the mouse click.</param>
/// <param name="key: ">The key of the keyboard that has been pressed.</param>
/// <returns></returns>
bool Engine::Event(int MSG, POINT Coordinates, unsigned int key)
{
    int TranslatedMSG = TranslateMSG(Coordinates, MSG, key);

    switch (this->GameMode)
    {
    case this->GAMEMODE::Menu:
    {
        switch (TranslatedMSG)
        {
        case TRANSLATEDMSG_NEWGAMEPVE:
        {
            this->GameMode = this->GAMEMODE::PVE;
            this->SetMode(this->GAMESTATUS::Deploying);
        }
        break;
        case TRANSLATEDMSG_NEWGAMEPVP:
        {
        }
        break;
        }
    }
    break;
    case this->GAMEMODE::PVE:
    {
        switch (this->GameStatus)
        {
        case GAMESTATUS::Deploying:
        {
            switch (TranslatedMSG)
            {
            case TRANSLATEDMSG_MOVESHIPL:
            {
                userField.MoveActiveShip(BF_MOVE_LEFT);
            }
            break;
            case TRANSLATEDMSG_MOVESHIPR:
            {
                userField.MoveActiveShip(BF_MOVE_RIGHT);
            }
            break;
            case TRANSLATEDMSG_MOVESHIPUP:
            {
                userField.MoveActiveShip(BF_MOVE_UP);
            }
            break;
            case TRANSLATEDMSG_MOVESHIPDOWN:
            {
                userField.MoveActiveShip(BF_MOVE_DOWN);
            }
            break;
            case TRANSLATEDMSG_DEPLOY:
            {
                buttonFieldDeploy.Deploy();
            }
            break;
            case TRANSLATEDMSG_ROTATE:
            {
                userField.RotateActiveShip();
            }
            break;
            default: 
                return MSG_VOID;
                break;
            }
        }
        break;
        case GAMESTATUS::MainGame:
        {
            switch (this->UserTurn)
            {
            case true:
            {
                switch (TranslatedMSG)
                {
                case TRANSLATEDMSG_RANDOMAIM:
                {
                    userField.SetAimPoint(enemyField.RandomSelect());
                }
                break;
                case TRANSLATEDMSG_AIM:
                {
                    userField.SetAimPoint(enemyField.Select(this->MSGParam.FieldCoordinates.x, this->MSGParam.FieldCoordinates.y));
                }
                break;
                case TRANSLATEDMSG_MOVE_LEFT:
                {
                    userField.SetAimPoint(enemyField.MoveSelection(BF_MOVE_LEFT));
                }
                break;
                case TRANSLATEDMSG_MOVE_RIGHT:
                {
                    userField.SetAimPoint(enemyField.MoveSelection(BF_MOVE_RIGHT));
                }
                break;
                case TRANSLATEDMSG_MOVE_DOWN:
                {
                    userField.SetAimPoint(enemyField.MoveSelection(BF_MOVE_DOWN));
                }
                break;
                case TRANSLATEDMSG_MOVE_UP:
                {
                    userField.SetAimPoint(enemyField.MoveSelection(BF_MOVE_UP));
                }
                break;
                case TRANSLATEDMSG_FIRE:
                {
                    this->Shoot(&userField, &enemyField);
                }
                break;
                }
            }
            break;
            case false:
            {
                this->Shoot(&enemyField, &userField);
            }
            break;
            }

        }
        break;
        default:
            return MSG_VOID;
            break;
        }
    }
    break;
    case this->GAMEMODE::PVP:
    {

    }
    break;
    }
    return true;
}

/// <summary>
/// Moves a ship from enemyField to userField.
/// </summary>
/// <param name="EnemyFieldShip: ">The ship to be given to userField from enemyField.</param>
/// <param name="UserFieldShip: ">The ship that will be recieving EnemyFieldShip.</param>
void Engine::MoveShipToUserField(Ship EnemyFieldShip, Ship& UserFieldShip)
{
    UserFieldShip = EnemyFieldShip;
    if (this->ShipsDeployed != 0)
        userField.Ships[this->ShipsDeployed - 1].Deployed = true;
    userField.Ships[this->ShipsDeployed].Deployed = false;
    userField.SetShipDeployableStatus();
}

void Engine::Shoot(Field* FieldFrom, Field* FieldTo)
{
    if (!FieldTo->CanFire()) return;
    const short int AnswerStatus = FieldTo->ShootRecieve(FieldFrom->ShootCreate());
    FieldFrom->ShootAnswer(AnswerStatus);
    if (AnswerStatus == this->ShootStatus::Miss) this->SwitchTurns();
}

/// <summary>
/// Sets the game mode.
/// </summary>
/// <param name="Mode: ">The new mode to be set.</param>
void Engine::SetMode(GAMESTATUS GameStatus)
{
    this->GameStatus = GameStatus;
    this->ShipsDeployed = 0;
    switch (GameStatus)
    {
    case GAMESTATUS::Deploying:
    {
        userField.CleanShips();
        userField.ClearField();
        enemyField.CreateShips(GameStatus);
        this->MoveShipToUserField(enemyField.Ships[this->ShipsDeployed], userField.Ships[this->ShipsDeployed]);
        enemyField.SetShipMarkers();
        userField.SetShipMarkers();
    }
    break;
    case GAMESTATUS::MainGame:
    {
        enemyField.CreateShips(GameStatus);
        userField.SetAimPoint(enemyField.RandomSelect());
    }
    }
}

/// <summary>
/// Translates the Message for Engine::Event.
/// </summary>
/// <param name="Coordinates: ">The coordinates of the mouse click.</param>
/// <param name="MSG: ">The untranslated message.</param>
/// <param name="Key: ">The key of the keyboard that has been pressed.</param>
/// <returns></returns>
int Engine::TranslateMSG(POINT Coordinates, const int MSG, const unsigned int Key)
{
    switch (this->GameStatus)
    {
    case GAMESTATUS::NewGame:
    {
        switch (MSG)
        {
        case MSG_LBTTNDOWN:
        {
            if(!buttonFieldNewGame.Click(Coordinates)) return MSG_VOID;
            this->MSGParam.FieldCoordinates = Coordinates;
            switch (buttonFieldNewGame.Cells[Coordinates.x][Coordinates.y].ButtonID)
            {
            case BF_NEWGAME_PVE:
                return TRANSLATEDMSG_NEWGAMEPVE;
            case BF_NEWGAME_PVP:
                return TRANSLATEDMSG_NEWGAMEPVP;
            default: return MSG_VOID;
            }
        }
        break;
        default: return MSG_VOID;
        }
    }
    break;
    case GAMESTATUS::Deploying:
    {
        switch(MSG)
        {
        case MSG_LBTTNDOWN:
        {
            if (userField.Click(Coordinates))
            {
                if (!userField.Ships[this->ShipsDeployed].Deployable) break;
                int TempShipID = userField.ShipExists(Coordinates, this->ShipsDeployed);
                if (TempShipID >= 0 && TempShipID != this->ShipsDeployed)
                    userField.SwapActiveShip(TempShipID);
            }
            else if (buttonFieldDeploy.Click(Coordinates))
            {
                this->MSGParam.FieldCoordinates = Coordinates;
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
                case BF_DEPLOY:
                    return TRANSLATEDMSG_DEPLOY;
                case BF_ROTATE:
                    return TRANSLATEDMSG_ROTATE;
                default: return MSG_VOID;
                }
            }
        }
        break;
        case MSG_KEYPRESS:
        {
            switch (Key)
            {
            case VK_DOWN:
                return TRANSLATEDMSG_MOVESHIPDOWN;
            case VK_LEFT:
                return TRANSLATEDMSG_MOVESHIPL;
            case VK_UP:
                return TRANSLATEDMSG_MOVESHIPUP;
            case VK_RIGHT:
                return TRANSLATEDMSG_MOVESHIPR;
            case 13:
                return TRANSLATEDMSG_DEPLOY;
            case 32:
                return TRANSLATEDMSG_ROTATE;
            default: return MSG_VOID;
            }
        }
        break;

        default: return MSG_VOID;
        }
    }
    break;
    case GAMESTATUS::MainGame:
    {
        switch (MSG)
        {
        case MSG_LBTTNDOWN:
        {
            if (enemyField.Click(Coordinates))
            {
                this->MSGParam.FieldCoordinates = Coordinates;
                return TRANSLATEDMSG_AIM;
            }
            if (buttonFieldFire.Click(Coordinates))
            {
                this->MSGParam.FieldCoordinates = Coordinates;
                switch (buttonFieldFire.Cells[Coordinates.x][Coordinates.y].ButtonID)
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
        break;
        case MSG_KEYPRESS:
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
            break;
        }
        default: return MSG_VOID;
        }
    }
    break;
    }
    return MSG_VOID;
}

void Engine::SwitchTurns()
{
    this->UserTurn = !this->UserTurn;
}
