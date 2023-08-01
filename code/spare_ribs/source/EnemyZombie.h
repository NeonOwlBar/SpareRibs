#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>
#include <time.h>
#include "ProjectileManager.h"
#include "Player.h"
#include "TiledMap.h"

class EnemyZombie
{
	friend class ProjectileManager;
public:
	EnemyZombie(SDL_Renderer* renderer, ProjectileManager* projectileManager, Player* player) : 
		renderer(renderer), projectileManager(projectileManager), player(player) {}
	~EnemyZombie() { clean(); }

	struct Zombie
	{
		int x, y;
		int zomSpeed = 2;
		int health = 2;
		int maxHealth = 2;
		int pixelWidth = 40;
		int unitFactor = pixelWidth / maxHealth;	//	100HP total, unit factor represents 1HP
		int val;
		bool hitWall = false;
		bool isAlive = true;
	};

	void init();
	void update();
	void render();
	void clean();

	void spawnWave();

	TiledMap* mapZom;

private:
	int zomSizeX = 60;
	int zomSizeY = 112;

	std::vector<Zombie> zombies;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* zombieTexture = nullptr;
	ProjectileManager* projectileManager;
	Player* player;
	const int hitTimerMS = 500;
	unsigned int lastHit = 0;
	const int MAX_ZOMBIES = 6;
	int currentZomNum;

	const int FRAME_COUNT = 5;
	const int TILESHEET_WIDTH = 5;
	int animFrame = 0;
	int ANIMATION_SPEED = 250; // number of ms between frames
	long int lastAnimChange = 0;

	int zombiesDefeated = 0;
};

