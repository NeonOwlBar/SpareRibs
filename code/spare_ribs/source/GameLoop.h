#pragma once

#include <iostream>
#include <string>
#include <SDL.h>
#include "Player.h"
#include "FontRenderer.h"
#include "TiledMap.h"
#include "ProjectileManager.h"
#include "EnemyZombie.h"
//#include "MainMenu.h"

const int screenWidth = 800;
const int screenHeight = 600;

class GameLoop
{
public:
	int init();

	void menuUpdate();
	void menuRender();
	bool isMainMenu();

	void update();
	void render();
	bool keepAlive();

	void clean();

private:

	//MainMenu* mainMenu = nullptr;

	SDL_Window* window = nullptr;
	SDL_Surface* screenSurface = nullptr;
	SDL_Renderer* renderer = nullptr;
	std::unique_ptr<FontRenderer> fontRenderer;	// smart pointer, goes onto heap instead of stack
	TiledMap* tiledMap = nullptr;
	Player* player = nullptr;
	ProjectileManager* pm;
	EnemyZombie* zombie;

};

