#include "Engine.h"
#include <string>
#include <typeinfo>
#include <math.h>
#include "UserField.h"
#include "EnemyField.h"
#include "ButtonFieldDeploy.h"
#include "ButtonFieldFire.h"
#include "ButtonFieldConnect.h"
#include "ButtonFieldNewGame.h"
#include "textureManager.h"

extern const float OpenGLHeight;
extern const float OpenGLWidth;
extern const float AspectRatio;

extern UserField userField;
extern EnemyField enemyField;
extern ButtonFieldDeploy buttonFieldDeploy;
extern ButtonFieldFire buttonFieldFire;
extern ButtonFieldConnect buttonFieldConnect;
extern ButtonFieldNewGame buttonFieldNewGame;
extern TextureManager textureManager;

// define while only PVE game is avaliable
#define GAMEMODE_PVE_ONLY

/// <summary>
/// Default constructor for engine class.
/// </summary>
Engine::Engine() :GameStatus(GAMESTATUS::NewGame), lastGameResults(LastGameResults::N_A),
fOffsetH(0), fOffsetW(0), fCurrentHeight(0), fCurrentWidth(0), fGLUnitSize(0),
ShipsDeployed(0), UserTurn(true), Animation(false), FrameCount(0),
MatchTimeSec(0), PlayerShipsAlive(10), OpponentShipsAlive(10)
{
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    this->dtn = tp.time_since_epoch();
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
            this->StartNewGame();
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
            if (!this->Animation)
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
            else
            {

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

    POINT Aimpoint = FieldFrom->ShootCreate();

    this->StartAnimation(FieldTo, Aimpoint);

    const short int AnswerStatus = FieldTo->ShootRecieve(Aimpoint);
    FieldFrom->ShootAnswer(AnswerStatus);
    if (AnswerStatus > 0)
    {
        if (typeid(*FieldFrom) == typeid(EnemyField))
        {
            this->PlayerShipsAlive--;
            if (this->PlayerShipsAlive == 0) this->GameOver(false);
        }
        else
        {
            this->OpponentShipsAlive--;
            if (this->OpponentShipsAlive == 0) this->GameOver(true);
        }
    }
    if (AnswerStatus == this->ShootStatus::Miss) this->SwitchTurns();
}

void Engine::IncreaseMatchTime()
{
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::chrono::system_clock::duration dtn = tp.time_since_epoch();
    if (dtn.count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den >
        this->dtn.count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den)
    {
        this->dtn = dtn;
        if (this->GameStatus == this->GAMESTATUS::MainGame)
            if (++this->MatchTimeSec == 6000) this->MatchTimeSec = 0;
    }
}

void Engine::StartNewGame()
{
    enemyField.NewGameReset();
    this->MatchTimeSec = 0;
    this->ShipsDeployed = 0;
    this->PlayerShipsAlive = 10;
    this->OpponentShipsAlive = 10;
    this->UserTurn = true;
}

void Engine::GameOver(bool UserWon)
{
    this->SetMode(this->GAMESTATUS::NewGame);
    switch (UserWon)
    {
    case true:
    {
        this->lastGameResults = Engine::LastGameResults::UserWon;
    }
    break;
    case false:
    {
        this->lastGameResults = Engine::LastGameResults::OpponentWon;
    }
    break;
    }
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
    case GAMESTATUS::NewGame:
    {
        this->GameMode = this->GAMEMODE::Menu;
    }
    break;
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
    break;
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

void Engine::StartAnimation(Field* field, POINT ShootingPoint)
{
    this->Animation = true;
    this->ShootPoint = { (float)field->StartX + ShootingPoint.x, (float)field->StartY + ShootingPoint.y };
    if (typeid(*field) == typeid(UserField))
    {
        this->UserShot = false;
        this->ShootingAngle = -0.3 + (ShootingPoint.y * 0.06);
        ShootPoint.y += 0.3;
        for (int i = 0; i < 30; i++)
        {
            CannonBallPositionsX[i] = 28.5 - ((28.5 - ShootPoint.x) / (float)30) * i;
            CannonBallPositionsY[i] = ((ShootPoint.y - 10) / (float)(ShootPoint.x - 28.5)) 
                * (CannonBallPositionsX[i] - 28.5) + 10;
        }
    }
    else
    {
        this->UserShot = true;
        this->ShootingAngle = 3.44 - (ShootingPoint.y * 0.06);
        ShootPoint.x += 0.5;
        ShootPoint.y += 0.3;
        for (int i = 0; i < 30; i++)
        {
            CannonBallPositionsX[i] = 3.5 + ((ShootPoint.x - 3.5) / (float)30) * i;
            CannonBallPositionsY[i] = ((ShootPoint.y - 10) / (float)(ShootPoint.x - 3.5))
                * (CannonBallPositionsX[i] - 3.5) + 10;
        }
    }
}

void Engine::DrawAnimation()
{
    const unsigned int num_segments = 360;
    float theta = 0;
    float angleincrease = 1;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureManager.RocketBodyTextureID);

    glBegin(GL_QUADS);

    glTexCoord2d(0, 0); glVertex2f(this->CannonBallPositionsX[FrameCount], this->CannonBallPositionsY[FrameCount]);
    glTexCoord2d(1.f, 0); glVertex2f(0.7f + this->CannonBallPositionsX[FrameCount], this->CannonBallPositionsY[FrameCount]);
    glTexCoord2d(1.f, 1.f); glVertex2f(0.7f + this->CannonBallPositionsX[FrameCount], 0.3f + this->CannonBallPositionsY[FrameCount]);
    glTexCoord2d(0, 1.f); glVertex2f(this->CannonBallPositionsX[FrameCount], 0.3f + this->CannonBallPositionsY[FrameCount]);
    
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureManager.RocketTopTextureID);

    glBegin(GL_TRIANGLES);

    glTexCoord2d(0, 0); glVertex2f(this->CannonBallPositionsX[FrameCount]+0.7f, this->CannonBallPositionsY[FrameCount]);
    glTexCoord2d(1.f, 0.5f); glVertex2f(this->CannonBallPositionsX[FrameCount]+1, this->CannonBallPositionsY[FrameCount]+0.15f);
    glTexCoord2d(0, 1.f); glVertex2f(this->CannonBallPositionsX[FrameCount]+0.7f, this->CannonBallPositionsY[FrameCount]+0.3f);

    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureManager.RocketFinTopTextureID);

    glBegin(GL_TRIANGLES);

    glTexCoord2d(0, 0); glVertex2f(this->CannonBallPositionsX[FrameCount], this->CannonBallPositionsY[FrameCount]+0.3f);
    glTexCoord2d(1.f, 0); glVertex2f(this->CannonBallPositionsX[FrameCount] + 0.36f, this->CannonBallPositionsY[FrameCount] + 0.3f);
    glTexCoord2d(0, 1.f); glVertex2f(this->CannonBallPositionsX[FrameCount], this->CannonBallPositionsY[FrameCount] + 0.39f);

    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureManager.RocketFinBottomTextureID);

    glBegin(GL_TRIANGLES);

    glTexCoord2d(0, 0); glVertex2f(this->CannonBallPositionsX[FrameCount], this->CannonBallPositionsY[FrameCount]-0.093f);
    glTexCoord2d(1.f, 1.f); glVertex2f(this->CannonBallPositionsX[FrameCount] + 0.36f, this->CannonBallPositionsY[FrameCount]);
    glTexCoord2d(0, 1.f); glVertex2f(this->CannonBallPositionsX[FrameCount], this->CannonBallPositionsY[FrameCount]);

    glEnd();
    glDisable(GL_TEXTURE_2D);

    if (++this->FrameCount == 30)
    {
        Animation = false;
        this->FrameCount = 0;
    }
}

