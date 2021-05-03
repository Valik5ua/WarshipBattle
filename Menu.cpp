#include "recource.h"
#include "Menu.h"

/// <summary>
/// This function creates the menus
/// </summary>
/// <param name="hwnd:">Handle to the window of interest</param>
void CreateMenus(HWND hwnd)
{
    //Menus
    HMENU hMenu = CreateMenu(); //Highest level Menu Bar
    HMENU HMenuGame = CreateMenu(); // Top level Menu for the "New Game" menu
    HMENU HMenuNewPVE = CreateMenu(); // Middle Level Menu from HMenuHewGame, specifies Player VS Computer
    HMENU HMenuHelp = CreateMenu(); // Top Level Menu for the "Help" menu

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)HMenuGame, L"Game");


    AppendMenu(HMenuGame, MF_POPUP, (UINT_PTR)HMenuNewPVE, L"Player Vs Computer");

    AppendMenu(HMenuNewPVE, MF_STRING, (UINT_PTR)MENU_GAME_EASY, L"Easy Mode");
    AppendMenu(HMenuNewPVE, MF_STRING, (UINT_PTR)MENU_GAME_MEDIUM, L"Medium Mode");
    AppendMenu(HMenuNewPVE, MF_STRING, (UINT_PTR)MENU_GAME_HARD, L"Hard Mode");

    AppendMenu(HMenuGame, MF_STRING, (UINT_PTR)MENU_GAME_PVP, L"Player Vs Player (Network)");
    
    AppendMenu(HMenuGame, MF_STRING, (UINT_PTR)MENU_GAME_EXIT, L"Exit");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)HMenuHelp, L"Help");

    AppendMenu(HMenuHelp, MF_STRING, (UINT_PTR)MENU_HELP_ABOUT, L"About");

    SetMenu(hwnd, hMenu);
}
