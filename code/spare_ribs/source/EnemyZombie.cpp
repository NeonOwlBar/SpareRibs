#include "EnemyZombie.h"

void EnemyZombie::init()
{
	SDL_Surface* zombieSprite = IMG_Load("assets/ZombieAnims.png");
	if (zombieSprite == NULL)
	{
		std::cout << "Could not initialise zombie sprite!" << std::endl;
		return;
	}
	this->zombieTexture = SDL_CreateTextureFromSurface(this->renderer, zombieSprite);
	if (this->zombieTexture == NULL)
	{
		std::cout << "Could not initialise zombie texture!" << std::endl;
		return;
	}
	SDL_FreeSurface(zombieSprite);
	lastAnimChange = SDL_GetTicks();

	srand(time(NULL));

	for (int i = 0; i < MAX_ZOMBIES; i++)
	{
		// initiliased with default values
		Zombie zombie;
		// assigning undefined values
		/*zombie.x = rand() % (250 - zomSizeX) + 500;
		zombie.y = rand() % (500 - zomSizeY) + 50;
		zombie.zomSpeed = rand() % 2 + 1;
		zombie.val = i + 1;*/
		zombies.push_back(zombie);
	}
	spawnWave();
}

void EnemyZombie::update()
{
	if (zombiesDefeated >= MAX_ZOMBIES)
	{
		spawnWave();
	}

	for (auto &z : zombies)
	{
		// skips to next zombie if current zombie is defeated
		if (!z.isAlive) continue;

		z.x += z.zomSpeed;

		// full rect around zombie
		SDL_Rect zombieRect = { z.x + 10, z.y, zomSizeX - 20, zomSizeY };
		SDL_Rect nullRect;
		for (auto& r : projectileManager->ribs)
		{
			SDL_Rect ribRect = { r.x, r.y, 36, 27 };
			if (SDL_IntersectRect(&ribRect, &zombieRect, &nullRect) && (r.velocity))
			{
				r.velocity = 0;
				z.health -= 1;
				if (z.health <= 0)
				{
					z.zomSpeed = 0;
					z.x = 801;	// this is checked later to move the zombie
					z.isAlive = false;
					zombiesDefeated += 1;
					player->setScore(1);
				}
			}
		}


		SDL_Rect playerRect = { player->getX(), player->getY(), 60, 120 };
		if (SDL_GetTicks() - lastHit > hitTimerMS)	// provides invincibility frames
		{
			if (SDL_IntersectRect(&playerRect, &zombieRect, &nullRect))
			{
				player->setHealth(-5);
				lastHit = SDL_GetTicks();
			}
		}

		// allows zombies to fit through smaller gaps
		zombieRect = { z.x + 10, z.y + (zomSizeY / 3), zomSizeX - 20, zomSizeY * 2 / 3 };
		z.hitWall = false;
		for (int i = 0; i < 12; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				if (z.hitWall == false) {
					if (mapZom->collisionLvl1[i][j] == 1)
					{
						SDL_Rect mapTile = { j * 50, i * 50, 50, 50 };
						if (SDL_HasIntersection(&zombieRect, &mapTile))
						{
							z.zomSpeed *= -1;
							z.hitWall = true;
						}
					}
				}
			}
		}
	}

	auto remove = std::remove_if(zombies.begin(), zombies.end(),
		[](const Zombie& z)
		{
			return z.x > 1000;
		});
	zombies.erase(remove, zombies.end());

	if (SDL_GetTicks() - lastAnimChange < ANIMATION_SPEED)
	{
		return;
	}
	lastAnimChange = SDL_GetTicks();
	animFrame++;
	if (animFrame >= FRAME_COUNT)
	{
		animFrame = 1;
	}
	
	// storage for zombies.size() prior to loop. 
	// Required so zombies vector does not overflow
	currentZomNum = zombies.size();
	if (currentZomNum < MAX_ZOMBIES)	// if no. of zombies < max allowed
	{
		for (int i = 0; i < (MAX_ZOMBIES - currentZomNum); i++)
		{
			zombies.push_back(Zombie{ 801, 50, 0 }); // xPos, yPos, speed
		}
	}
}

void EnemyZombie::render()
{
	for (auto &z : zombies)
	{
		// skips to next zombie if current is defeated
		if (!z.isAlive) continue;
		int hpWidth = z.health * z.unitFactor;

		SDL_Rect healthBackground = { z.x + 10, (z.y - 10), z.pixelWidth, 7 };	// red background bar
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &healthBackground);
		SDL_Rect healthBar = { z.x + 10, (z.y - 10), hpWidth, 7 };	// green bar indicating health value
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &healthBar);

		SDL_Rect dest = { z.x, z.y, zomSizeX, zomSizeY };
		int animX = 80 * (animFrame % TILESHEET_WIDTH);
		int animY = 155 * (animFrame / TILESHEET_WIDTH);
		SDL_Rect src = { animX, animY, 80, 155 };
		if (z.zomSpeed < 0)
		{
			SDL_RenderCopyEx(renderer, zombieTexture, &src, &dest, 0, NULL, SDL_FLIP_HORIZONTAL);
		}
		else
		{
			SDL_RenderCopyEx(renderer, zombieTexture, &src, &dest, 0, NULL, SDL_FLIP_NONE);
		}
	}
}

void EnemyZombie::clean()
{
	SDL_DestroyTexture(this->zombieTexture);
	SDL_DestroyRenderer(this->renderer);

	mapZom = nullptr;
	projectileManager = nullptr;

	delete mapZom;
	delete projectileManager;
}

void EnemyZombie::spawnWave()
{
	zombiesDefeated = 0;
	for (auto &z : zombies) {
		z.x = rand() % (250 - zomSizeX) + 500;
		z.y = rand() % (500 - zomSizeY) + 50;
		z.health = z.maxHealth;
		z.zomSpeed = rand() % 2 + 1;
		z.isAlive = true;
	}
}