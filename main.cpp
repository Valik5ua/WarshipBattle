#include "GL/glew.h"
#include "GL/freeglut.h" 
#include "WindowsX.h"
#include <string>
#include "Menu.h"

#define DEBUG

// Windows globals
CHAR   WindowClassName[] = { "Windows OpenGL" };
HWND   hwnd{};
HDC    hDC{};
HGLRC  hRC{};

// Custom globals
bool   TimeToRedraw{};
HANDLE TimerFuncHandler{};
float  FrameRate = (float)1000 / 60;
Engine _Engine;

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

// main window procedure 
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
        _Engine.ConvertPixelsToGL(&ClickCoordinate);
        
        std::string Msg = "(" + std::to_string(ClickCoordinate.x)+","
            + std::to_string(ClickCoordinate.y) + ")";
        MessageBoxA(hWnd, Msg.c_str(), "Click coordinates:", MB_OK);
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
// OpenGL code

GLvoid Resize(GLsizei width, GLsizei height)
{
    _Engine.SetWindowGLParam(width, height);
    glViewport(0, 0, width, height);
    glLoadIdentity(); //Reset Coordinate System
    gluOrtho2D(-(_Engine.GetOffsetW()), OpenGLWidth + _Engine.GetOffsetW(), -(_Engine.GetOffsetH()), OpenGLHeight + _Engine.GetOffsetH()); //Setting Up 2D ORTHOGRAPHIC projection
    glMatrixMode(GL_MODELVIEW); //Changing back mode to MODELVIEW mode to start drawing
    DrawScene();
}

GLvoid InitGL(GLsizei width, GLsizei height)
{
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
    
    _Engine.SetWindowGLParam(width, height);
    glViewport(0, 0, width, height);
    glLoadIdentity(); //Reset Coordinate System
    gluOrtho2D(-(_Engine.GetOffsetW()), OpenGLWidth + _Engine.GetOffsetW(), -(_Engine.GetOffsetH()), OpenGLHeight + _Engine.GetOffsetH()); //Setting Up 2D ORTHOGRAPHIC projection
    glMatrixMode(GL_MODELVIEW); //Changing back mode to MODELVIEW mode to start drawing
}

GLvoid DrawScene(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glLoadIdentity();

    glColor3f(0, 1, 0);
    glVertex2f(0, 0);
    glVertex2f(OpenGLWidth, 0);
    glVertex2f(OpenGLWidth, OpenGLHeight);
    glVertex2f(0, OpenGLHeight);
    glEnd();
    for (int i{}; i < OpenGLWidth; i++)
    {
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(i, 0);
        glVertex2f(i, OpenGLHeight);
    }
    for (int i{}; i < OpenGLHeight; i++)
    {
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(0, i);
        glVertex2f(OpenGLWidth, i);
    }
    glEnd();
    SWAPBUFFERS;
}

//Custom Functions

/// <summary>
/// Function that tells the program when to redraw the window
/// </summary>
/// <param name="">No paraneters required</param>
void TimerFunc(LPVOID)
{
    while (true)
    {
        Sleep(FrameRate);
        TimeToRedraw = true;
    }
}
