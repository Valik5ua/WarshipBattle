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
#include "StatusField.h"
#include "ClueField.h"
#include "resource.h"
#include <thread>
#include "SoundButton.h"

extern const float OpenGLHeight;
extern const float OpenGLWidth;
extern const float AspectRatio;

extern Engine engine;
extern UserField userField;
extern EnemyField enemyField;
extern ButtonFieldDeploy buttonFieldDeploy;
extern ButtonFieldFire buttonFieldFire;
extern ButtonFieldConnect buttonFieldConnect;
extern ButtonFieldNewGame buttonFieldNewGame;
extern TextureManager textureManager;
extern StatusField statusField;
extern ClueField clueField;
extern SoundButton soundButton;

/// <summary>
/// Default constructor for engine class.
/// </summary>
Engine::Engine() :GameStatus(GAMESTATUS::NewGame), lastGameResults(LastGameResults::N_A),
fOffsetH(0), fOffsetW(0), fCurrentHeight(0), fCurrentWidth(0), fGLUnitSize(0),
ShipsDeployed(0), UserTurn(true), animation(Animation::None), LastShotAccomplished(true),
MatchTimeSec(0), PlayerShipsAlive(10), OpponentShipsAlive(10)
{
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	this->dtn = tp.time_since_epoch();
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

	if (TranslatedMSG == TRANSLATEDMSG_SOUNDBUTTONCLICK)
	{
		switch (soundButton.State)
		{
		case SoundButton::STATE::On:
		{
			soundButton.State = SoundButton::STATE::Off;
		}
		break;
		case SoundButton::STATE::Off:
		{
			soundButton.State = SoundButton::STATE::On;
		}
		break;
		}
	}

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
			if (this->animation==Animation::None)
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
						if (this->LastShotAccomplished) this->Shoot(&userField, &enemyField);
					}
					break;
					}
				}
				break;
				case false:
				{
					if(this->LastShotAccomplished) this->Shoot(&enemyField, &userField);
				}
				break;
				}
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
	if (!FieldTo->CanFire())
	{
		if (soundButton.State == SoundButton::STATE::On)
		{
			PlaySound(NULL, NULL, NULL);
			PlaySound(MAKEINTRESOURCE(S_WAVE_ERROR), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
		}
		return;
	}

	if (soundButton.State == SoundButton::STATE::On)
	{
		PlaySound(NULL, 0, 0);
		PlaySound(MAKEINTRESOURCE(S_WAVE_SHOOT), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
	}

	this->LastShotAccomplished = false;

	POINT Aimpoint = FieldFrom->ShootCreate();

	this->StartAnimation(FieldTo, Aimpoint);

	const short int AnswerStatus = FieldTo->ShootRecieve(Aimpoint);
	FieldFrom->ShootAnswer(AnswerStatus);
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

void Engine::DecreaseShipsAlive(bool User)
{
	if (User)
	{
		if (--this->PlayerShipsAlive == 0)
		{
			this->GameOver(false);
			Sleep(600);

			if (soundButton.State == SoundButton::STATE::On)
			{
				PlaySound(NULL, NULL, NULL);
				PlaySound(MAKEINTRESOURCE(S_WAVE_LOSE), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
			}
		}
	}
	else
	{
		if (--this->OpponentShipsAlive == 0)
		{
			this->GameOver(true);
			Sleep(600);

			if (soundButton.State == SoundButton::STATE::On)
			{
				PlaySound(NULL, NULL, NULL);
				PlaySound(MAKEINTRESOURCE(S_WAVE_WIN), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
			}
		}
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

	this->animation = Animation::MainMenu;
	this->menuAnimation.DefaultDirection = false;

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
		enemyField.GameOver();
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
		this->animation = Animation::MainMenu;
		this->menuAnimation.DefaultDirection = true;
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
	if (MSG == MSG_LBTTNDOWN)
	{
		if (soundButton.Click(Coordinates))
		{
			return TRANSLATEDMSG_SOUNDBUTTONCLICK;
		}
	}
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
	this->animation = Animation::Rocket;
	this->rocket.ShootPoint = { (float)field->StartX + ShootingPoint.x, (float)field->StartY + ShootingPoint.y + 0.5f };
	if (typeid(*field) == typeid(UserField))
	{
		this->UserShot = false;
		float TempX = this->rocket.ShootPoint.x - EnemyCannonFieldPosX;
		float TempY = ShootingPoint.y - ((EnemyGameFieldH - 1) / (float)2);
		float Res = TempY / TempX;
		this->ShootingAngle = 0 - atan(Res);
		for (int i = 0; i < this->rocket.FramesToDraw; i++)
		{
			this->rocket.Position[i].x = (EnemyCannonFieldPosX + 1) - (((EnemyCannonFieldPosX + 1) - this->rocket.ShootPoint.x) / (float)this->rocket.FramesToDraw) * (i + 1);
			this->rocket.Position[i].y = ((this->rocket.ShootPoint.y - (EnemyCannonFieldPosY + 1)) / (float)(this->rocket.ShootPoint.x - (EnemyCannonFieldPosX + 1)))
				* (this->rocket.Position[i].x - (EnemyCannonFieldPosX + 1)) + (EnemyCannonFieldPosY + 1);
		}
	}
	else
	{
		this->UserShot = true;
		float TempX = this->rocket.ShootPoint.x - (UserCannonFieldPosX + 2);
		float TempY = ShootingPoint.y - ((UserGameFieldH - 1) / (float)2);
		float Res = TempY / TempX;
		this->ShootingAngle = 3.14 - atan(Res);
		for (int i = 0; i < this->rocket.FramesToDraw; i++)
		{
			this->rocket.Position[i].x = (UserCannonFieldPosX + 1) + ((this->rocket.ShootPoint.x - (UserCannonFieldPosX + 1)) / (float)this->rocket.FramesToDraw) * (i + 1);
			this->rocket.Position[i].y = ((this->rocket.ShootPoint.y - (UserCannonFieldPosY + 1)) / (float)(this->rocket.ShootPoint.x - (UserCannonFieldPosX + 1)))
				* (this->rocket.Position[i].x - (UserCannonFieldPosX + 1)) + (UserCannonFieldPosY + 1);
		}
	}
}

void Engine::AnimationRocket::Draw()
{
	if (this->FrameCount < this->FramesToDraw - 1)
	{
		this->FrameCount++;

		if (this->FrameCount < 3)
		{
			const unsigned int num_segments = 360;
			float theta = 0;
			float angleincrease = 1;

			glPushMatrix();
			glTranslatef(this->Position[1].x, this->Position[1].y, 0);
			glScaled((this->FrameCount / 2), this->FrameCount / 2, 1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureManager.ExplosionTextureID);

			glBegin(GL_TRIANGLE_FAN);
			for (int i = theta; i < num_segments; i++)
			{
				float x = (float)116 / (float)128 * cosf(theta); //calculate current x in the segment
				float y = (float)116 / (float)128 * sinf(theta); //calculate current y in the segment

				glTexCoord2d(.5 + cosf(theta) / 2, .5 + sinf(theta) / 2); glVertex2f(x, y);

				theta += angleincrease;
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}

		glPushMatrix();
		glTranslatef(this->Position[this->FrameCount].x, this->Position[this->FrameCount].y, 0); //Set middle
		
		float TempAngle = 180 - engine.ShootingAngle * 180 / 3.14;
		glRotatef(TempAngle, 0, 0, 1.0f);

		const unsigned int num_segments = 360;
		float theta = 0;
		const float angleincrease = 1;

			glPushMatrix();

			glTranslatef(-0.5f, 0, 0);

			if (this->FrameCount % 2 == 0)
				glScaled(.18f, .18f, 1);
			else
				glScaled(.3f, .3f, 1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureManager.ExplosionTextureID);

			glBegin(GL_TRIANGLE_FAN);
			for (int i = theta; i < num_segments; i++)
			{
				float x = (float)116 / (float)128 * cosf(theta); //calculate current x in the segment
				float y = (float)116 / (float)128 * sinf(theta); //calculate current y in the segment

				glTexCoord2d(.5 + cosf(theta) / 2, .5 + sinf(theta) / 2); glVertex2f(x, y);

				theta += angleincrease;
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureManager.RocketBodyTextureID);
		glBegin(GL_QUADS);

		glTexCoord2d(0, 0); glVertex2f(-0.5f, -0.17f);
		glTexCoord2d(1.f, 0); glVertex2f(0.2f, -0.17f);
		glTexCoord2d(1.f, 1.f); glVertex2f(0.2f, 0.17f);
		glTexCoord2d(0, 1.f); glVertex2f(-0.5f, 0.17f);

		glEnd();
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureManager.RocketTopTextureID);

		glBegin(GL_TRIANGLES);

		glTexCoord2d(0, 0); glVertex2f(0.2f, -0.17f);
		glTexCoord2d(1.f, 0.5f); glVertex2f(0.5f, 0.0f);
		glTexCoord2d(0, 1.f); glVertex2f(0.2f, 0.17f);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureManager.RocketFinTopTextureID);

		glBegin(GL_TRIANGLES);

		glTexCoord2d(0, 0); glVertex2f(-0.5f, 0.17f);
		glTexCoord2d(1.f, 0); glVertex2f(-0.14f, 0.17f);
		glTexCoord2d(0, 1.f); glVertex2f(-0.5, 0.26f);

		glEnd();
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureManager.RocketFinBottomTextureID);

		glBegin(GL_TRIANGLES);

		glTexCoord2d(0, 0); glVertex2f(-0.5f, -0.17f);
		glTexCoord2d(1.f, 1.f); glVertex2f(-0.14f, -0.17f);
		glTexCoord2d(0, 1.f); glVertex2f(-0.5f, -0.26f);

		glEnd();
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
	}
	else
	{

		const unsigned int num_segments = 360;
		float theta = 0;
		float angleincrease = 1;

		glPushMatrix();
		glTranslatef(this->ShootPoint.x + 0.5f, this->ShootPoint.y, 0);
		glScaled((this->FrameCount - this->FramesToDraw + 1) / 3.5f, (this->FrameCount - this->FramesToDraw + 1) / 3.5f, 1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureManager.ExplosionTextureID);

		glBegin(GL_TRIANGLE_FAN);
		for (int i = theta; i < num_segments; i++)
		{
			float x = (float)116 / (float)128 * cosf(theta); //calculate current x in the segment
			float y = (float)116 / (float)128 * sinf(theta); //calculate current y in the segment

			glTexCoord2d(.5 + cosf(theta) / 2, .5 + sinf(theta) / 2); glVertex2f(x, y);

			theta += angleincrease;
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		if (++this->FrameCount == this->FramesToDraw + 5)
		{
			engine.animation = Animation::None;
			this->FrameCount = 0;
		}
	}
}

Engine::MenuAnimation::MenuAnimation() :FrameCount(0), DefaultDirection(true)
{
	for (int i = 0; i < this->FramesToDraw; i++)
	{
		this->StatusFieldPositionsX[i] = StatusFieldPosX + ((StatusFieldMainGamePosX - StatusFieldPosX) / ((float)FramesToDraw - 1)) * i;
		this->ClueFieldPositionsX[i] = ClueFieldPosX - ((ClueFieldPosX - ClueFieldMainGamePosX) / ((float)FramesToDraw - 1)) * i;
	}
}

void Engine::MenuAnimation::Draw()
{
	switch(this->DefaultDirection)
	{
	case true:
	{
		if (this->FrameCount < this->FramesToDraw)
		{
			clueField.startX = this->ClueFieldPositionsX[this->FrameCount];
			statusField.startX = this->StatusFieldPositionsX[this->FrameCount];
			
			this->FrameCount++;
		}
		else
		{
			clueField.startX = ClueFieldMainGamePosX;
			statusField.startX = StatusFieldMainGamePosX;

			engine.animation = Animation::None;
			this->FrameCount = 0;
		}
	}
	break;
	case false:
	{
		if (this->FrameCount < this->FramesToDraw)
		{
			clueField.startX = this->ClueFieldPositionsX[this->FramesToDraw - this->FrameCount];
			statusField.startX = this->StatusFieldPositionsX[this->FramesToDraw - this->FrameCount];
			
			this->FrameCount++;
		}
		else
		{
			clueField.startX = ClueFieldPosX;
			statusField.startX = StatusFieldPosX;

			engine.animation = Animation::None;
			this->FrameCount = 0;
		}
	}
	break;
	}
}
