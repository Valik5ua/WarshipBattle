#include "WindowsX.h"
#include "Menu.h"
#include "Engine.h"
#include "UserField.h"
#include "EnemyField.h"
#include "ButtonFieldDeploy.h"
#include "ButtonFieldFire.h"
#include "ButtonFieldConnect.h"
#include "ButtonFieldNewGame.h"
#include "TextureManager.h"
#include "StatisticField.h"
#include "StatusField.h"
#include "CannonField.h"
#include "ClueField.h"
#include "resource.h"

// Windows globals

CHAR   WindowClassName[] = { "Windows OpenGL" };
HWND   hwnd{};
HDC    hDC{};
HGLRC  hRC{};
HINSTANCE hInst{};

// Custom globals
bool   TimeToRedraw{};
HANDLE TimerFuncHandler{};
const float  FrameRate = (float)1000 / 60;
Engine engine;
ButtonFieldDeploy buttonFieldDeploy(ButtonFieldDeployPosX, ButtonFieldDeployPosY);
ButtonFieldFire buttonFieldFire(ButtonFieldFirePosX, ButtonFieldDeployPosY);
ButtonFieldConnect buttonFieldConnect(ButtonFieldConnectPosX, ButtonFieldConnectPosY);
ButtonFieldNewGame buttonFieldNewGame(ButtonFieldNewGamePosX, ButtonFieldNewGamePosY);
UserField userField(UserFieldPosX,UserFieldPosY);
EnemyField enemyField(EnemyFieldPosX, EnemyFieldPosY);
TextureManager textureManager;
StatisticField statisticField(StatisticFieldPosX,StatisticFieldPosY);
StatusField statusField(StatusFieldPosX, StatusFieldPosY);
ClueField clueField(ClueFieldPosX, ClueFieldPosY);
CannonField UserCannonField(UserCannonFieldPosX, UserCannonFieldPosY,3.14);
CannonField OpponentCannonField(EnemyCannonFieldPosX, EnemyCannonFieldPosY,0);

//Windows prototypes

LONG WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL B_SetupPixelFormat(HDC);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

//OpenGL defines 

#define SWAPBUFFERS SwapBuffers(hDC) 

// OpenGL prototypes 

GLvoid Resize(GLsizei, GLsizei);
GLvoid InitGL(GLsizei, GLsizei);
GLvoid DrawScene(GLvoid);

//Custom prototypes

void TimerFunc(LPVOID);

/// <summary>
/// The starting point of the program.
/// </summary>
/// <param name="hInstance: ">A handle to the current instance of the application.</param>
/// <param name="hPrevInstance: ">A handle to the previous instance of the application. (Always supposed to be NULL)</param>
/// <param name="lpCmdLine: ">The command line for the application, excluding the program name.</param>
/// <param name="nShowCmd: ">Controls how the window is to be shown.</param>
/// <returns>A value of 1 (0x1) when succesful.</returns>
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	hInst = hInstance;

	MSG        msg;
	WNDCLASS   wndclass;

	/* Register the frame class */
	wndclass.style = 0;
	wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = (LPCWSTR)WindowClassName;
	wndclass.lpszClassName = (LPCWSTR)WindowClassName;

	if (!RegisterClass(&wndclass))
		return FALSE;

	/* Create the frame */
	hwnd = CreateWindow((LPCWSTR)WindowClassName,
		L"Warships Battle",
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		MinimalWidth,
		MinimalHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	/* make sure window was created */
	if (!hwnd)
		return FALSE;

	/* show and update main window */
	ShowWindow(hwnd, SW_SHOWMAXIMIZED);

	UpdateWindow(hwnd);
	
	//Loading Textures into memory
	
	//Create thread to Implement TimerFunc, which tells the program when to redraw the window 
	TimerFuncHandler = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)TimerFunc, NULL, NULL, NULL);
	
	/* animation loop */
	while (true) {

		// Process all pending messages 

		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				return TRUE;
			}
		}
		if (TimeToRedraw)
		{
			engine.Event(MSG_VOID);
			engine.IncreaseMatchTime();
			DrawScene();
			TimeToRedraw = false;
		}
	}
}

/// <summary>
/// Main loop.
/// </summary>
/// <param name="hWnd: ">Handle to the main window.</param>
/// <param name="uMsg: ">Message identifier.</param>
/// <param name="wParam: ">First message parameter.</param>
/// <param name="lParam: ">Second message parameter.</param>
/// <returns>A value of 1 unless the default case is activated.</returns>
LONG WINAPI MainWndProc(
	HWND    hWnd,
	UINT    uMsg,
	WPARAM  wParam,
	LPARAM  lParam)
{
	LONG    lRet = 1;
	PAINTSTRUCT    ps;
	RECT rect;

	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case MENU_HELP_ABOUT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, About);
		}
		break;
		case MENU_GAME_EXIT:
			SendMessage(hWnd, WM_CLOSE, NULL, NULL);
			break;
		case MENU_GAME_PVE:
		{
			engine.StartNewGame();
			engine.GameMode = engine.GAMEMODE::PVE;
			engine.SetMode(engine.GAMESTATUS::Deploying);

			clueField.startX = ClueFieldPosX;
			statusField.startX = StatusFieldPosX;
		}
		break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		break;
	case WM_CREATE:
		hDC = GetDC(hWnd);
		if (!B_SetupPixelFormat(hDC))
			PostQuitMessage(0);

		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		GetClientRect(hWnd, &rect);
		InitGL(rect.right, rect.bottom);
		CreateMenus(hWnd);
		break;
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_SIZE:
		GetClientRect(hWnd, &rect);
		Resize(rect.right, rect.bottom);
		break;
	case WM_LBUTTONDOWN:
	{
		POINT ClickCoordinate{ GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam) };
		engine.ConvertPixelsToGL(&ClickCoordinate);
		engine.Event(MSG_LBTTNDOWN, ClickCoordinate);
	}
	break;
	case WM_KEYDOWN:
	{
		engine.Event(MSG_KEYPRESS, { NULL, NULL }, wParam);
	}
	break;
	case WM_CLOSE:
		if (hRC)
			wglDeleteContext(hRC);
		if (hDC)
			ReleaseDC(hWnd, hDC);
		hRC = 0;
		hDC = 0;
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		if (hRC)
			wglDeleteContext(hRC);
		if (hDC)
			ReleaseDC(hWnd, hDC);
		CloseHandle(TimerFuncHandler);
		PostQuitMessage(0);
		break;
	case WM_GETMINMAXINFO:
	{
		PMINMAXINFO pMinMaxInfo{ (PMINMAXINFO)lParam };
		pMinMaxInfo->ptMinTrackSize.x = MinimalWidth;
		pMinMaxInfo->ptMinTrackSize.y = MinimalHeight;
	}
	break;
	default:
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return lRet;
}

/// <summary>
/// Sets up Pixel Format.
/// </summary>
/// <param name="hdc: ">A handle to the current DC.</param>
/// <returns>True if success, else false.</returns>
BOOL B_SetupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd, * ppfd;
	int pixelformat;

	ppfd = &pfd;

	ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
	ppfd->nVersion = 1;
	ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	ppfd->dwLayerMask = PFD_MAIN_PLANE;
	ppfd->iPixelType = PFD_TYPE_COLORINDEX;
	ppfd->cColorBits = 8;
	ppfd->cDepthBits = 16;
	ppfd->cAccumBits = 0;
	ppfd->cStencilBits = 0;

	pixelformat = ChoosePixelFormat(hdc, ppfd);

	if ((pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0)
	{
		MessageBoxA(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE)
	{
		MessageBoxA(NULL, "SetPixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	return TRUE;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

/// <summary>
/// A function that is activated whenever the window is being resized.
/// </summary>
/// <param name="width: ">The desired width of the window.</param>
/// <param name="height: ">The desired Height of the window.</param>
/// <returns>A value of void.</returns>
GLvoid Resize(GLsizei width, GLsizei height)
{
	engine.SetWindowGLParam(width, height);
	glViewport(0, 0, width, height);
	glLoadIdentity(); //Reset Coordinate System
	gluOrtho2D(-(engine.GetOffsetW()), OpenGLWidth + engine.GetOffsetW(), -(engine.GetOffsetH()), OpenGLHeight + engine.GetOffsetH()); //Setting Up 2D ORTHOGRAPHIC projection
	glMatrixMode(GL_MODELVIEW); //Changing back mode to MODELVIEW mode to start drawing
	DrawScene();
}

/// <summary>
/// Initializes OpenGL.
/// </summary>
/// <param name="width: ">The desired starting width of the window.</param>
/// <param name="height: ">The desired starting height of the window.</param>
/// <returns>A value of void.</returns>
GLvoid InitGL(GLsizei width, GLsizei height)
{
	//sets backround color
	glClearColor(0.3f, 0.3f, 1.0f, 1.0f);

	//load textures into memory
	textureManager.LoadAllTextures();

	//Setting screen offset + size info
	engine.SetWindowGLParam(width, height);
	glViewport(0, 0, width, height);
	glLoadIdentity(); //Reset Coordinate System
	gluOrtho2D(-(engine.GetOffsetW()), OpenGLWidth + engine.GetOffsetW(), -(engine.GetOffsetH()), OpenGLHeight + engine.GetOffsetH()); //Setting Up 2D ORTHOGRAPHIC projection
	glMatrixMode(GL_MODELVIEW); //Changing back mode to MODELVIEW mode to start drawing
	engine.SetMode(engine.GameStatus);
}

/// <summary>
/// Draws everything onscreen except backround.
/// </summary>
/// /// <param name="">No parameters required.</param>
/// <returns>A value of void</returns>
GLvoid DrawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT);
	userField.Draw();
	enemyField.Draw();
	clueField.Draw();
	statisticField.Draw();
	statusField.Draw();
	switch (engine.GameStatus)
	{
	case Engine::GAMESTATUS::NewGame:
	{
		buttonFieldNewGame.Draw();
	}
	break;
	case Engine::GAMESTATUS::Deploying:
	{
		buttonFieldDeploy.Draw();
	}
	break;
	case Engine::GAMESTATUS::MainGame:
	{
		buttonFieldFire.Draw();
	}
	break;
	}

	switch (engine.animation)
	{
	case Engine::Animation::None:
	{
		UserCannonField.Draw();
		OpponentCannonField.Draw();
	}
	break;
	case Engine::Animation::Rocket:
	{
		if (engine.UserShot)
		{
			UserCannonField.Draw(engine.ShootingAngle);
			OpponentCannonField.Draw();
		}
		else
		{
			OpponentCannonField.Draw(engine.ShootingAngle);
			UserCannonField.Draw();
		}
		engine.rocket.Draw();
	}
	break;
	case Engine::Animation::MainMenu:
	{
		engine.menuAnimation.Draw();
		UserCannonField.Draw();
		OpponentCannonField.Draw();
	}
	break;
	default:
		break;
	}

	SWAPBUFFERS;
}

/// <summary>
/// Function that tells the program when to redraw the window.
/// </summary>
/// <param name="">No parameters required.</param>
void TimerFunc(LPVOID)
{
	while (true)
	{
		Sleep(FrameRate);
		TimeToRedraw = true;
	}
}
