#pragma once

#include <SDL.h>
#include <SDL_image.h>
//vector for managing arrays
#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>

#include "Player.h"
#include "TiledMap.h"


class ProjectileManager
{
	friend class EnemyZombie;	// gives EnemyZombie access to private variables
public:
	ProjectileManager(SDL_Renderer* renderer, Player* player) : renderer(renderer), player(player) {}

	struct Rib
	{
		float x, y, rotation, distance;
		int velocity = 8;
		bool isThrowable = true;
	};

	void init();
	void processInput();
	void update();
	void render();
	void clean();

	

	int getMouseX();
	int getMouseY();

	TiledMap* mapRib;

private:
	SDL_Renderer* renderer;
	SDL_Texture* ribTexture;
	std::vector<Rib> ribs;
	Player* player;

	

	// time limit between shots
	const int SHOOT_TIMER_MS = 500;
	// number of pixels travelled per frame
	unsigned int lastShot = 0;

	int ribSizeX = 36;
	int ribSizeY = 27;

	int windowWidth = 800;
	int windowHeight = 600;

	int mouseX, mouseY;
};

