// spare_ribs.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "GameLoop.h"
//#include "MainMenu.h"

// Preprocessor directive to only have console appear when in debug config
// 
// To undo this, only use int main() (remove int WinMain(), along with preprocessor directives), and in project properties do the following:
//      - in Release config
//      - Linker > System > Subsystem
//      - set Subsystem to Console
// 
//      - This is set to Windows inside Release config to remove console.

#ifdef _DEBUG
int main()
#else
int WinMain()
#endif
{
    GameLoop gameLoop = GameLoop();
    //MainMenu mainMenu = MainMenu();
    if (gameLoop.init() < 0)
    {
        return 1;
    }
    //while (gameLoop.isMainMenu()) 
    //{
    //    /*mainMenu.menuUpdate();
    //    mainMenu.menuRender();*/
    //}
    while (gameLoop.keepAlive())
    {
        gameLoop.update();
        gameLoop.render();
    }

    gameLoop.clean();
    return 0;
}