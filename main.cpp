/*
 * Example of a Windows OpenGL program.
 * The OpenGL code is the same as that used in
 * the X Window System sample
 */
#include "GL/glew.h"
#include "GL/freeglut.h" 

 /* Windows globals, defines, and prototypes */
CHAR szAppName[] = "Win OpenGL";
HWND  ghWnd;
HDC   ghDC;
HGLRC ghRC;

#define SWAPBUFFERS SwapBuffers(ghDC) 
#define BLACK_INDEX     0 
#define WIDTH           900 
#define HEIGHT          600
#define ID_SM_Linear 1
#define ID_QUADRATIC 2
#define ID_SM_Sin 3
#define ID_SM_Cos 4
#define ID_SM_Tan 5
#define ID_SM_EXIT 6
#define ID_SM_ABOUT 7
#define ID_SM_Help 8

LONG WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL bSetupPixelFormat(HDC);

/* OpenGL globals, defines, and prototypes */
GLfloat latinc, longinc;
bool drawtime;
GLdouble radius;
HANDLE HTimerFunc;
float refreshRate = (float)1000 / 60;
GLvoid resize(GLsizei, GLsizei);
GLvoid initializeGL(GLsizei, GLsizei);
GLvoid drawScene(GLvoid);

void timerFunc(LPVOID)
{
    while (true)
    {
        Sleep(refreshRate);
        drawtime = true;
    }
}

void CreateMenus(HWND hwnd)
{

}
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    MSG        msg;
    WNDCLASS   wndclass;

    /* Register the frame class */
    wndclass.style = 0;
    wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIconA(hInstance, szAppName);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndclass.lpszMenuName = (LPCWSTR)szAppName;
    wndclass.lpszClassName = (LPCWSTR)szAppName;

    if (!RegisterClass(&wndclass))
        return FALSE;

    /* Create the frame */
    ghWnd = CreateWindow((LPCWSTR)szAppName,
        L"Windows OpenGL",
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WIDTH,
        HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL);

    /* make sure window was created */
    if (!ghWnd)
        return FALSE;

    /* show and update main window */
    ShowWindow(ghWnd, nShowCmd);

    UpdateWindow(ghWnd);

    /* animation loop */
    HTimerFunc = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)timerFunc, NULL, NULL, NULL);
    while (1) {
        /*
         *  Process all pending messages
         */

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
        if (drawtime)
        {
            drawScene();
            drawtime = false;
        }
        //longinc += (GLfloat)0.00001;
    }
}

/* main window procedure */
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
        case ID_SM_EXIT:
        {
            SendMessage(hWnd, WM_CLOSE, NULL, NULL);
        }
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        break;
    case WM_CREATE:
        ghDC = GetDC(hWnd);
        if (!bSetupPixelFormat(ghDC))
            PostQuitMessage(0);

        ghRC = wglCreateContext(ghDC);
        wglMakeCurrent(ghDC, ghRC);
        GetClientRect(hWnd, &rect);
        initializeGL(rect.right, rect.bottom);
        CreateMenus(hWnd);
        break;
    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_SIZE:
        GetClientRect(hWnd, &rect);
        resize(rect.right, rect.bottom);
        break;

    case WM_CLOSE:
        if (ghRC)
            wglDeleteContext(ghRC);
        if (ghDC)
            ReleaseDC(hWnd, ghDC);
        ghRC = 0;
        ghDC = 0;

        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        if (ghRC)
            wglDeleteContext(ghRC);
        if (ghDC)
            ReleaseDC(hWnd, ghDC);
        TerminateThread(HTimerFunc, 0x0);
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
            longinc -= 0.05F;
            break;
        case VK_RIGHT:
            longinc += 0.05F;
            break;
        case VK_UP:
            latinc += 0.5F;
            break;
        case VK_DOWN:
            latinc -= 0.5F;
            break;
        }

    default:
        lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }

    return lRet;
}

BOOL bSetupPixelFormat(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd, * ppfd;
    int pixelformat;

    ppfd = &pfd;

    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;
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

/* OpenGL code */

GLvoid resize(GLsizei width, GLsizei height)
{

    GLfloat aspect;

    glViewport(0, 0, width, height);

    aspect = (GLfloat)width / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 3.0, 7.0);
    glMatrixMode(GL_MODELVIEW);
}
GLvoid initializeGL(GLsizei width, GLsizei height)
{
    GLfloat     maxObjectSize, aspect;
    GLdouble    near_plane, far_plane;

    glClearIndex((GLfloat)BLACK_INDEX);
    glClearDepth(1.0);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    aspect = (GLfloat)width / height;
    gluPerspective(45.0, aspect, 3.0, 7.0);
    glMatrixMode(GL_MODELVIEW);

    near_plane = 3.0;
    far_plane = 7.0;
    maxObjectSize = 3.0F;
    radius = near_plane + maxObjectSize / 2.0;

    latinc = 6.0F;
    longinc = 0.0F;
}

GLvoid drawScene(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 1);
    glVertex3f(longinc, 0.1f, -4);
    glVertex3f(longinc, 0.3f, -4);
    glVertex3f(longinc, 0.5f, -4);
    glEnd();
    SWAPBUFFERS;
}