// cplusplus_programming_for_games.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "GameLoop.h"
#include "MainMenu.h"

int main()
{
    GameLoop gameLoop = GameLoop();
    MainMenu mainMenu = MainMenu();
    if (gameLoop.init() < 0)
    {
        return 1;
    }
    while (gameLoop.isMainMenu()) 
    {
        /*mainMenu.menuUpdate();
        mainMenu.menuRender();*/
    }
    while (gameLoop.keepAlive())
    {
        gameLoop.update();
        gameLoop.render();
    }

    gameLoop.clean();
    return 0;
}