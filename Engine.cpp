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
#include <stdio.h>

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
Engine::Engine()	:GameMode(GAMEMODE::Menu),
					GameStatus(GAMESTATUS::NewGame),
					animation(Animation::None),
					lastGameResults(LastGameResults::N_A),
					connection(nullptr),
					fOffsetH(0),
					fOffsetW(0),
					fCurrentHeight(0),
					fCurrentWidth(0),
					fGLUnitSize(0),
					ShipsDeployed(0),
					UserTurn(true),
					LastShotAccomplished(true),
					MatchTimeSec(0),
					PlayerShipsAlive(10),
					OpponentShipsAlive(10)
{
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	this->dtn = tp.time_since_epoch();

	this->netChecker.Connected = true;
	this->netChecker.CheckingAttemptsFailed = 0;
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
		switch (this->GameStatus)
		{
		case GAMESTATUS::NewGame:
		{
			switch (TranslatedMSG)
			{
			case TRANSLATEDMSG_NEWGAMEPVE:
			{
				this->StartNewGame();
				this->GameMode = GAMEMODE::PVE;
				this->SetStatus(GAMESTATUS::Deploying);
			}
			break;
			case TRANSLATEDMSG_NEWGAMEPVP:
			{
				this->StartNewGame();
				this->GameStatus = GAMESTATUS::ChoosingConnectionMode;
			}
			break;
			}
		}
		break;
		case GAMESTATUS::ChoosingConnectionMode:
		{
			switch (TranslatedMSG)
			{
			case TRANSLATEDMSG_CONNECTION_AUTO:
			{
				this->GameStatus = GAMESTATUS::AutoConnection;
			}
			break;
			case TRANSLATEDMSG_CONNECTION_MANUAL:
			{
				this->GameStatus = GAMESTATUS::ChoosingConnectionSide;
			}
			break;
			case TRANSLATEDMSG_CONNECTION_CANCEL:
			{
				this->GameMode = GAMEMODE::Menu;
				this->GameStatus = GAMESTATUS::NewGame;
			}
			break;
			}
		}
		break;
		case GAMESTATUS::ChoosingConnectionSide:
		{
			switch (TranslatedMSG)
			{
			case TRANSLATEDMSG_CONNECTION_SERVER:
			{
				this->GameStatus = GAMESTATUS::ServerConnection;
			}
			break;
			case TRANSLATEDMSG_CONNECTION_CLIENT:
			{
				this->GameStatus = GAMESTATUS::ClientConnection;
			}
			break;
			case TRANSLATEDMSG_CONNECTION_CANCEL:
			{
				this->GameMode = GAMEMODE::Menu;
				this->GameStatus = GAMESTATUS::NewGame;
			}
			break;
			}
		}
		break;
		case GAMESTATUS::ClientConnection:
		{
			switch (TranslatedMSG)
			{
			case TRANSLATEDMSG_CONNECTION_INPUTIP:
			{
				this->GameStatus = GAMESTATUS::ClientConnection;
				//InputIP();
			}
			break;
			case TRANSLATEDMSG_CONNECTION_CANCEL:
			{
				this->GameMode = GAMEMODE::Menu;
				this->GameStatus = GAMESTATUS::NewGame;
			}
			break;
			}
		}
		break;
		case GAMESTATUS::ServerConnection:
		{
			switch (TranslatedMSG)
			{
			case TRANSLATEDMSG_CONNECTION_SHOWIP:
			{
				this->GameStatus = GAMESTATUS::ClientConnection;
				//ShowIP();
			}
			break;
			case TRANSLATEDMSG_CONNECTION_CANCEL:
			{
				this->GameMode = GAMEMODE::Menu;
				this->GameStatus = GAMESTATUS::NewGame;
			}
			break;
			}
		}
		break;
		case GAMESTATUS::AutoConnection:
		{
			if (!connection)
			{
				connection = new Connection(UDP::ConnectionType::AUTO);
				this->connection->AsyncAutoConnect();
			}
			if (this->connection->Connected())
			{
				this->SetStatus(GAMESTATUS::Deploying);
				this->GameMode = GAMEMODE::PVP;
			}
			switch (TranslatedMSG)
			{
			case TRANSLATEDMSG_CONNECTION_CANCEL:
			{
				this->connection->AsyncDisconnect();
				this->GameStatus = GAMESTATUS::Disconnecting;
			}
			break;
			}
		}
		break;
		case GAMESTATUS::Disconnecting:
		{
			if (connection->Disconnected())
			{
				delete this->connection;
				this->connection = nullptr;
				this->GameMode = GAMEMODE::Menu;
				this->GameStatus = GAMESTATUS::NewGame;
			}
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
				if (this->ShipsDeployed == 10)
				{
					engine.SetStatus(Engine::GAMESTATUS::MainGame);
				}
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
		this->connection->SendMSG(TYPE_CHECK, FLAG_ONE, (char*)"Check");
		UDP::MSG Msg;
		if (this->connection->ReceiveMSG(Msg, 2))
		{
			if (Msg.TYPE == TYPE_CHECK && Msg.FLAG == FLAG_ONE)
			{
				netChecker.CheckingFunc(true);
			}
			else if (Msg.TYPE == TYPE_DEPLOYING && Msg.FLAG == FLAG_ONE)
			{
				this->ShipsMSG(Msg.msg);
				this->OpponentIsReady = true;
			}
			else
			{
				netChecker.CheckingFunc(false);
				if (!netChecker.Connected)
				{
					this->CloseConnection();
					MessageBoxA(hwnd, "Disconnected from opponent.", "Disconnected.", NULL);
					this->StartNewGame();

					clueField.startX = ClueFieldPosX;
					statusField.startX = StatusFieldPosX;
				}
			}
		}
		else
		{ 
			netChecker.CheckingFunc(false);
			if (!netChecker.Connected)
			{
				this->CloseConnection();
				MessageBoxA(hwnd, "Disconnected from opponent.", "Disconnected.", NULL);
				this->StartNewGame();

				clueField.startX = ClueFieldPosX;
				statusField.startX = StatusFieldPosX;
			}
		}
		
		switch (this->GameStatus)
		{
		case GAMESTATUS::Deploying:
		{
			if (engine.ShipsDeployed == 10)
			{
				if (this->OpponentIsReady)
				{
					this->SetStatus(GAMESTATUS::MainGame);

					if (this->connection->Connected(UDP::ConnectionType::SERVER)) this->UserTurn = true;
					else this->UserTurn = false;
				}
			}
			else
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
					if (engine.ShipsDeployed == 10)
					{
						std::string StrMsg = this->ShipsMSG();

						//const int Len = StrMsg.size() + 1;
						char* MSGToSend = &StrMsg[0];

						//for (int i = 0; i < StrMsg.size(); i++)
						//{
						//	MSGToSend[i] = StrMsg[i];
						//}

						//std::copy(StrMsg.begin(), StrMsg.end(), MSGToSend);
						//MSGToSend[Len - 1] = '\0';

						connection->SendMSG(TYPE_DEPLOYING, FLAG_ONE, MSGToSend);
						//delete MSGToSend;
					}
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
		}
		break;
		}
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
	userField.CleanShips();
	userField.ClearField();

	enemyField.NewGameReset();

	this->MatchTimeSec = 0;
	this->ShipsDeployed = 0;
	this->PlayerShipsAlive = 10;
	this->OpponentShipsAlive = 10;
	this->OpponentIsReady = false;
	this->UserTurn = true;
}

void Engine::GameOver(bool UserWon)
{
	this->SetStatus(this->GAMESTATUS::NewGame);

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
/// Sets the GAMESTATUS.
/// </summary>
/// <param name="GameStatus: ">The new GAMESTATUS to be set.</param>
void Engine::SetStatus(GAMESTATUS GameStatus)
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
	case GAMESTATUS::ChoosingConnectionMode:
	{
		if (!buttonFieldConnect.Click(Coordinates)) return MSG_VOID;
		this->MSGParam.FieldCoordinates = Coordinates;
		switch (buttonFieldConnect.Cells[Coordinates.x][Coordinates.y].ButtonID)
		{
		case BF_CONNECT_TOP_BUTTON:
		{
			return TRANSLATEDMSG_CONNECTION_AUTO;
		}
		break;
		case BF_CONNECT_MIDDLE_BUTTON:
		{
			return TRANSLATEDMSG_CONNECTION_MANUAL;
		}
		break;
		case BF_CONNECT_BOTTOM_BUTTON:
		{
			return TRANSLATEDMSG_CONNECTION_CANCEL;
		}
		break;
		}
	}
	case GAMESTATUS::ChoosingConnectionSide:
	{
		if (!buttonFieldConnect.Click(Coordinates)) return MSG_VOID;
		this->MSGParam.FieldCoordinates = Coordinates;
		switch (buttonFieldConnect.Cells[Coordinates.x][Coordinates.y].ButtonID)
		{
		case BF_CONNECT_TOP_BUTTON:
		{
			return TRANSLATEDMSG_CONNECTION_SERVER;
		}
		break;
		case BF_CONNECT_MIDDLE_BUTTON:
		{
			return TRANSLATEDMSG_CONNECTION_CLIENT;
		}
		break;
		case BF_CONNECT_BOTTOM_BUTTON:
		{
			return TRANSLATEDMSG_CONNECTION_CANCEL;
		}
		break;
		}
	}
	break;
	case GAMESTATUS::ServerConnection:
	{
		if (!buttonFieldConnect.Click(Coordinates)) return MSG_VOID;
		this->MSGParam.FieldCoordinates = Coordinates;
		switch (buttonFieldConnect.Cells[Coordinates.x][Coordinates.y].ButtonID)
		{
		case BF_CONNECT_TOP_BUTTON:
		{
			return MSG_VOID;
		}
		break;
		case BF_CONNECT_MIDDLE_BUTTON:
		{
			return TRANSLATEDMSG_CONNECTION_SHOWIP;
		}
		break;
		case BF_CONNECT_BOTTOM_BUTTON:
		{
			return TRANSLATEDMSG_CONNECTION_CANCEL;
		}
		break;
		}
	}
	break;
	case GAMESTATUS::ClientConnection:
	{
		if (!buttonFieldConnect.Click(Coordinates)) return MSG_VOID;
		this->MSGParam.FieldCoordinates = Coordinates;
		switch (buttonFieldConnect.Cells[Coordinates.x][Coordinates.y].ButtonID)
		{
		case BF_CONNECT_TOP_BUTTON:
		{
			return MSG_VOID;
		}
		break;
		case BF_CONNECT_MIDDLE_BUTTON:
		{
			return TRANSLATEDMSG_CONNECTION_INPUTIP;
		}
		break;
		case BF_CONNECT_BOTTOM_BUTTON:
		{
			return TRANSLATEDMSG_CONNECTION_CANCEL;
		}
		break;
		}
	}
	break;
	case GAMESTATUS::AutoConnection:
	{
		if (!buttonFieldConnect.Click(Coordinates)) return MSG_VOID;
		this->MSGParam.FieldCoordinates = Coordinates;
		switch (buttonFieldConnect.Cells[Coordinates.x][Coordinates.y].ButtonID)
		{
		case BF_CONNECT_TOP_BUTTON:
		{
			return MSG_VOID;
		}
		break;
		case BF_CONNECT_MIDDLE_BUTTON:
		{
			return TRANSLATEDMSG_CONNECTION_SHOWIP;
		}
		break;
		case BF_CONNECT_BOTTOM_BUTTON:
		{
			return TRANSLATEDMSG_CONNECTION_CANCEL;
		}
		break;
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

void Engine::CloseConnection()
{
	if (this->connection)
	{
		this->connection->AsyncDisconnect();
		this->GameStatus = GAMESTATUS::Disconnecting;
		this->GameMode = GAMEMODE::Menu;
	}
}

void Engine::WaitForDisconnection()
{
	if (!connection) return;
	while (true)
	{
		if (this->connection->Disconnected())
		{
			delete this->connection;
			this->connection = nullptr;
			return;
		}
	}
}

void Engine::ShipsMSG(char* RecievedMSG)
{
	unsigned int MSGPos = 0;

	for (int i = 0; i < MAX_SHIPS_COUNT; i++)
	{
		this->RecievedShips[i].Size = ((int)RecievedMSG[MSGPos]) - 48;
		MSGPos++;
		this->RecievedShips[i].Rotated = ((int)RecievedMSG[MSGPos]) - 48;
		MSGPos++;

		switch (this->RecievedShips[i].Size)
		{
		case 1:
			this->RecievedShips[i].Decks[0].Type = Deck::DeckType::Single;
			break;
		case 2:
			this->RecievedShips[i].Decks[0].Type = Deck::DeckType::Back;
			this->RecievedShips[i].Decks[1].Type = Deck::DeckType::Front;
			break;
		case 3:
			this->RecievedShips[i].Decks[0].Type = Deck::DeckType::Back;
			this->RecievedShips[i].Decks[1].Type = Deck::DeckType::Middle;
			this->RecievedShips[i].Decks[2].Type = Deck::DeckType::Front;
			break;
		case 4:
			this->RecievedShips[i].Decks[0].Type = Deck::DeckType::Back;
			this->RecievedShips[i].Decks[1].Type = Deck::DeckType::Middle;
			this->RecievedShips[i].Decks[2].Type = Deck::DeckType::Middle;
			this->RecievedShips[i].Decks[3].Type = Deck::DeckType::Front;
			break;
		}

		for (int j = 0; j < this->RecievedShips[i].Size; j++)
		{
			this->RecievedShips[i].Decks[j].Position.x = ((int)RecievedMSG[MSGPos]) - 48;
			MSGPos++;
			this->RecievedShips[i].Decks[j].Position.y = ((int)RecievedMSG[MSGPos]) - 48;
			MSGPos++;
		}
	}
}

std::string Engine::ShipsMSG()
{
	const unsigned short int BytesToSend = 60;
	char MSG[BytesToSend + 1]{};
	unsigned int MSGPos = 0;

	for (int i = 0; i < MAX_SHIPS_COUNT; i++)
	{
		MSG[MSGPos] = ((char)userField.Ships[i].Size) + 48;
		MSGPos++;
		MSG[MSGPos] = ((char)userField.Ships[i].Rotated) + 48;
		MSGPos++;

		for (int j = 0; j < userField.Ships[i].Size; j++)
		{
			MSG[MSGPos] = ((char)userField.Ships[i].Decks[j].Position.x) + 48;
			MSGPos++;
			MSG[MSGPos] = ((char)userField.Ships[i].Decks[j].Position.y) + 48;
			MSGPos++;
		}
	}

	MSG[BytesToSend] = '\0';

	std::string ReturnMSG{};

	for (int i = 0; i < BytesToSend; i++)
	{
		ReturnMSG += MSG[i];
	}

	return ReturnMSG;
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

void Engine::NetChecker::CheckingFunc(bool success)
{
	if (success) { this->CheckingAttemptsFailed = 0; this->Connected = true; }
	else
	{
		if (++this->CheckingAttemptsFailed == this->MaxCheckingFails)
			this->Connected = false;
	}
}
