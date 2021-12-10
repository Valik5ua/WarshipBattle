#include "Menu.h"
#include <Windows.h>

/// <summary>
/// This function creates the menus.
/// </summary>
/// <param name="hwnd:">Handle to the window of interest.</param>
void CreateMenus(HWND hwnd)
{
    //Menus
    HMENU hMenu = CreateMenu(); //Highest level Menu Bar
    HMENU HMenuGame = CreateMenu(); // Top level Menu for the "New Game" menu
    HMENU HMenuHelp = CreateMenu(); // Top Level Menu for the "Help" menu

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)HMenuGame, L"Game");

    AppendMenu(HMenuGame, MF_STRING, (UINT_PTR)MENU_GAME_MAINMENU, L"New Game");
    
    AppendMenu(HMenuGame, MF_STRING, (UINT_PTR)MENU_GAME_EXIT, L"Exit");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)HMenuHelp, L"Help");

    AppendMenu(HMenuHelp, MF_STRING, (UINT_PTR)MENU_HELP_ABOUT, L"About");

    SetMenu(hwnd, hMenu);
}
