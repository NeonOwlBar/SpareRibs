// #define SDL_MAIN_HANDLED
#include "GameLoop.h"
#include <SDL.h>

int GameLoop::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "could not intialise SDL2!" << std::endl;
		std::cout << SDL_GetError() << std::endl;
		return -1;
	}
	window = SDL_CreateWindow(
		"Spare Ribs", 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		screenWidth, screenHeight, 
		SDL_WINDOW_SHOWN
	);
	if (window == NULL)
	{
		std::cout << "could not intialise window!" << std::endl;
		std::cout << SDL_GetError() << std::endl;
		return -1;
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		std::cout << "could not initialise renderer!" << std::endl;
		std::cout << SDL_GetError() << std::endl;
		return -1;
	}

	screenSurface = SDL_GetWindowSurface(window);

	if (SDL_Init(SDL_INIT_TIMER) != 0)
	{
		std::cout << "Could not initialise timer!" << std::endl;
		return 1;
	}

	mainMenu = new MainMenu();
	mainMenu->init(renderer);

	//instantiating objects
	player = new Player(renderer, screenWidth, screenHeight, "assets/RibAstleyWalking.png");
	player->init();

	fontRenderer = std::unique_ptr<FontRenderer>(new FontRenderer(renderer));
	fontRenderer->init();
	
	// tiledMap = std::unique_ptr<TiledMap>(new TiledMap(renderer, "assets/Dungeon_Tileset.png"));
	tiledMap = new TiledMap(renderer, "assets/Dungeon_Tileset.png");
	tiledMap->init();

	pm = new ProjectileManager(this->renderer, this->player);
	pm->init();
	
	zombie = new EnemyZombie(this->renderer, this->pm, this->player);
	zombie->init();

	player->mapPlayer = tiledMap;
	pm->mapRib = tiledMap;
	zombie->mapZom = tiledMap;

	return 0;
}

void GameLoop::menuUpdate()
{
	SDL_Delay(16);	// sets game to ~60 fps (1000/16 = 62.5)
}

void GameLoop::menuRender()
{
	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
}

bool GameLoop::isMainMenu()
{
	// consider making this a bool variable to check within keepAlive(), 
	// and add if statements into it to decide what to run depending on 
	// if main menu or game is active

	SDL_Event userInput;
	while (SDL_PollEvent(&userInput))
	{
		if (userInput.type == SDL_KEYDOWN)
		{
			switch (userInput.key.keysym.sym)
			{
			case SDLK_q:
				return false;
				break;
			case SDLK_LEFT:
				std::cout << "Left arrow key pressed" << std::endl;
				break;
			default:
				std::cout << "default case activated" << std::endl;
			}
		}

	}

	mainMenu->menuUpdate();
	mainMenu->menuRender();
	return mainMenu->menuAlive();
}

void GameLoop::update()
{
	player->update();
	pm->update();
	zombie->update();
	SDL_Delay(16);	// sets game to ~60 fps (1000/16 = 62.5)
}

void GameLoop::render()
{
	SDL_RenderClear(renderer);
	tiledMap->render();
	// renders string across entire window
	// fontRenderer->render("Hello :)");
	zombie->render();
	player->render();
	pm->render();
	SDL_RenderPresent(renderer);
}

bool GameLoop::keepAlive()
{
	if (player->getHealth() <= 0)
	{
		return false;
	}


	SDL_Event userInput;
	while (SDL_PollEvent(&userInput))
	{
		if (userInput.type == SDL_QUIT)
		{
			return false;
		}

		// checking for LMB
		if (userInput.type == SDL_MOUSEBUTTONDOWN)
		{
			if (SDL_BUTTON(1))
			{
				player->keyStates[SHOOT] = true;
				pm->processInput();
			}
		}
		if (userInput.type == SDL_MOUSEBUTTONUP)
		{
			if (SDL_BUTTON(1))
			{
				player->keyStates[SHOOT] = false;
			}
		}

		if (userInput.type == SDL_KEYDOWN)
		{
			switch (userInput.key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_a:
				player->keyStates[LEFT] = true;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				player->keyStates[RIGHT] = true;
				break;
			case SDLK_UP:
			case SDLK_w:
				player->keyStates[UP] = true;
				break;
			case SDLK_DOWN:
			case SDLK_s:
				player->keyStates[DOWN] = true;
				break;
			case SDLK_SPACE:
				player->keyStates[SHOOT] = true;
				pm->processInput();
				break;
			case SDLK_e:
				player->keyStates[INTERACT] = true;
				break;
			}
		}
		else if (userInput.type == SDL_KEYUP)
		{
			switch (userInput.key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_a:
				player->keyStates[LEFT] = false;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				player->keyStates[RIGHT] = false;
				break;
			case SDLK_UP:
			case SDLK_w:
				player->keyStates[UP] = false;
				break;
			case SDLK_DOWN:
			case SDLK_s:
				player->keyStates[DOWN] = false;
				break;
			case SDLK_SPACE:
				player->keyStates[SHOOT] = false;
				break;
			case SDLK_e:
				player->keyStates[INTERACT] = false;
				break;
			}
			player->setAnimFrame();	// resets player sprite to idle when movement button no longer pressed.
		}
	}

	return true;
}

void GameLoop::clean()
{
	fontRenderer->clean();
	tiledMap->clean();
	pm->clean();
	zombie->clean();

	delete player;
	delete tiledMap;
	delete pm;
	delete zombie;
	SDL_DestroyWindow(window);
	SDL_Quit();
}