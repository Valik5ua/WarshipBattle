#include "WindowsX.h"
#include "Menu.h"
#include "Engine.h"
#include "UserField.h"
#include "EnemyField.h"
#include "ButtonFieldDeploy.h"
#include "ButtonFieldFire.h"
#include "ButtonFieldConnect.h"
#include "TextureManager.h"

// Windows globals

CHAR   WindowClassName[] = { "Windows OpenGL" };
HWND   hwnd{};
HDC    hDC{};
HGLRC  hRC{};

// Custom globals

bool   TimeToRedraw{};
HANDLE TimerFuncHandler{};
float  FrameRate = (float)1000 / 60;
Engine engine;
ButtonFieldDeploy buttonFieldDeploy(3, 1);
ButtonFieldFire buttonFieldFire(3, 1);
ButtonFieldConnect buttonFieldConnect(3, 1);
UserField userField(3,5);
EnemyField enemyField(19, 5);
TextureManager textureManager;

Ship Ships[10]{};

//Windows prototypes

LONG WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL B_SetupPixelFormat(HDC);

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
	MSG        msg;
	WNDCLASS   wndclass;

	/* Register the frame class */
	wndclass.style = 0;
	wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIconA(hInstance, WindowClassName);
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
		case MENU_GAME_EXIT:
			SendMessage(hWnd, WM_CLOSE, NULL, NULL);
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
	engine.SetMode(engine.Mode);
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
	switch (engine.Mode)
	{
	case Engine::MODE::Deploying:
	{
		buttonFieldDeploy.Draw();
		userField.StartDeploying();
	}
	break;
	case Engine::MODE::MainGame:
	{
		buttonFieldFire.Draw();
	}
		break;
	case Engine::MODE::Connecting:
	{
		buttonFieldConnect.Draw();
	}
	}
	enemyField.Draw();

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
