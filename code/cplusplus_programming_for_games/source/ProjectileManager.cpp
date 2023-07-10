#include "ProjectileManager.h"

void ProjectileManager::init()
{
	// load the texture for the rib
	SDL_Surface* ribSprite = IMG_Load("assets/RibSingle.png");
	if (ribSprite == NULL)
	{
		std::cout << "Could not load rib sprite!" << std::endl;
		return;
	}
	this->ribTexture = SDL_CreateTextureFromSurface(this->renderer, ribSprite);
	if (ribTexture == NULL)
	{
		std::cout << "Could not create rib texture" << std::endl;
		return;
	}
	SDL_FreeSurface(ribSprite);
	
	// ribs is the vector used to contain all rib objects
	// push_back is the function in C++ which adds an element to the end of a vector
	// add six ribs to rib vector
	for (int i = 0; i < 6; i++) {
		ribs.push_back(Rib{ 900, 0, player->getAngle(), 0.0f, 0 });
	}
}

void ProjectileManager::processInput()
{
	if (player->keyStates[SHOOT] && (player->getAmmo() > 0))
	{
		// SDL_GetTicks() returns current time of game in ms
		if (SDL_GetTicks() - lastShot > SHOOT_TIMER_MS)
		{
			SDL_GetMouseState(&mouseX, &mouseY);
			player->setAngle(mouseX, mouseY);

			// ribs is the vector used to contain all rib objects
			// push_back is the function in C++ which adds an element to the end of a vector
			// ribs.push_back(Rib{ player->getX(), player->getY(), player->getAngle(), 0.0f });
			lastShot = SDL_GetTicks();

			/*for (auto& r : ribs) {
				r.x = player->getX();
				r.y = player->getY();
				r.rotation = player->getAngle();
				r.distance = 0.0f;
				r.velocity = 8;
			}*/
			int ammoLevel = player->getAmmo() - 1;
			ribs[ammoLevel].x = player->getX();
			ribs[ammoLevel].y = player->getY();
			ribs[ammoLevel].rotation = player->getAngle();
			ribs[ammoLevel].distance = 0.0f;
			ribs[ammoLevel].velocity = 8;

			player->setAmmo(-1);
			
		}
		std::cout << "Player ammo: " << player->getAmmo() << std::endl;
		std::cout << "Ribs size: " << ribs.size() << std::endl;
	}
}

void ProjectileManager::update()
{
	// auto keyword automatically defines type of variable based on type of variable assigned to it
	// For loop iterates through the vector and creates a reference to an instance (r) for each object
	for (auto& r : ribs)
	{
		//r.velocity = RIB_VELOCITY;
		// rotates the sprite depending on angle so the "top" of the sprite starts facing the direction of travel
		// e.g. angle = 45 degrees, X velocity = sin(45) = 0.707, Y velocity = cos(45) = 0.707
		// These values are then multiplied by RIB_VELOCITY to make the rib travel faster
		r.x += sin(r.rotation * PI / 180.0f) * r.velocity; // rotation * PI / 180  is to convert to radians
		r.y -= cos(r.rotation * PI / 180.0f) * r.velocity;
		r.distance += r.velocity;

		SDL_Rect ribRect = { r.x, r.y, ribSizeX, ribSizeY };
		SDL_Rect playerRect = { player->getX() - 10, player->getY() - 10, 80, 140 };
		SDL_Rect nullRect;
		if (SDL_IntersectRect(&ribRect, &playerRect, &nullRect) && (r.velocity == 0) && (player->keyStates[INTERACT]))
		{
			//r.distance = 1001;
			r.x = 900;
			player->setAmmo(1);
			std::cout << "Player ammo: " << player->getAmmo() << std::endl;
			std::cout << "Ribs size: " << ribs.size() << std::endl;
		}

		
		for (int i = 0; i < 12; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				if ((mapRib->collisionLvl1[i][j] == 1) || (mapRib->collisionLvl1[i][j] == 2))
				{
					SDL_Rect mapTile = { j * 50, i * 50, 50, 50 };
					if (SDL_HasIntersection(&ribRect, &mapTile))
					{
						r.velocity = 0;
					}
				}
			}
		}

	}

	// remove rib after they have travelled ~1000 pixels
	// remove_if creates a list of elements to be removed from rib array based on a condition
	// The condition is a pointer to a function which returns a bool value. 
		// In this case, the function is defined within the parameters of the remove_if function
	// remove_if(first, last, pred) -- pred returns a value convertible to bool. If true, it is removed
	auto remove = std::remove_if(ribs.begin(), ribs.end(),
		[](const Rib& r)
		{
			if (r.distance > 1000)
				std::cout << "rib removed" << std::endl;
			return r.distance > 1000;
		});
	ribs.erase(remove, ribs.end());
}

void ProjectileManager::render()
{
	// SDL_Point center = { 5, 5 };
	for (auto& r : ribs)
	{
		SDL_Rect dest = { r.x, r.y, ribSizeX, ribSizeY };	// third and fourth parameters of dest affect size of image
		// rendering function is SDL_RenderCopyEx which has extended parameters to include rotation
		SDL_RenderCopyEx(renderer, ribTexture, 0, &dest, r.rotation, NULL, SDL_FLIP_NONE);
	}
}

void ProjectileManager::clean()
{
	SDL_DestroyTexture(this->ribTexture);	// deletes texture which was allocated in the init function
}

int ProjectileManager::getMouseX()
{
	return mouseX;
}

int ProjectileManager::getMouseY()
{
	return mouseY;
}