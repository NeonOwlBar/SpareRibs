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
		// generates initial data for a rib
		Rib rib = Rib{ 900, 0, 0.0f, 0.0f, 0.0f, 0 };
		// ribs is the vector used to contain all rib objects
		// push_back is the function in C++ which adds an element to the end of a vector
		ribs.push_back(rib);
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
			player->setThrowAngle(mouseX, mouseY);

			lastShot = SDL_GetTicks();

			for (auto& r : ribs) {
				// checks if ribs is throwable (not already thrown)
				if (r.isThrowable == true) {
					r.isThrowable = false;
					// these are only called once per frame due to "break"
					r.x = player->getX();
					r.y = player->getY() + player->getHeight() / 2;
					r.throwRotation = player->getThrowAngle();
					r.spinRotation = r.throwRotation;
					r.distance = 0.0f;
					r.velocity = 8;
					break;	// ensures this only applies to the first 
							// rib in vector which is throwable
				}
			}

			player->setAmmo(-1);
		}
	}
}

void ProjectileManager::update()
{
	// updates playerRect for frame
	playerRect = { player->getX() - 10, player->getY() - 10, 80, 140 };

	// auto keyword automatically defines type of variable based on type of variable assigned to it
	// For loop iterates through the vector and creates a reference to an instance (r) for each object
	for (auto& r : ribs)
	{
		r.ribRect = { r.x, r.y, ribSizeX, ribSizeY };

		// includes code only required when rib is in mid-air
		if (r.velocity != 0) {

			// rotates the sprite depending on angle so the "top" of the sprite starts facing the direction of travel
			// e.g. angle = 45 degrees, X velocity = sin(45) = 0.707, Y velocity = cos(45) = 0.707
			// These values are then multiplied by RIB_VELOCITY to make the rib travel faster
			r.x += sin(r.throwRotation * PI / 180.0f) * r.velocity; // rotation * PI / 180  is to convert to radians
			r.y -= cos(r.throwRotation * PI / 180.0f) * r.velocity;
			r.distance += r.velocity;

			// makes rib always look like its thrown forward
			if (r.throwRotation > 0 || r.throwRotation < -180) {
				r.spinRotation += 10;
			}
			else {
				r.spinRotation -= 10;
			}

			for (int i = 0; i < 12; i++)
			{
				for (int j = 0; j < 16; j++)
				{
					if (mapRib->collisionLvl1[i][j] == 1)
					{
						mapTile = { j * 50, i * 50, 50, 50 };
						if (SDL_HasIntersection(&r.ribRect, &mapTile))
						{
							r.velocity = 0;
						}
					}
				}
			}
		}

		if (SDL_IntersectRect(&r.ribRect, &playerRect, &nullRect) && (r.velocity == 0) && (player->keyStates[INTERACT]))
		{
			r.x = 900;
			r.isThrowable = true;
			player->setAmmo(1);
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
			return r.distance > 1000;
		});
	ribs.erase(remove, ribs.end());
}

void ProjectileManager::render()
{
	// SDL_Point center = { 5, 5 };
	for (auto& r : ribs)
	{
		ribDest = { r.x, r.y, ribSizeX, ribSizeY };
		// rendering function is SDL_RenderCopyEx which has extended parameters to include rotation
		SDL_RenderCopyEx(renderer, ribTexture, 0, &ribDest, r.spinRotation, NULL, SDL_FLIP_NONE);
	}
}

void ProjectileManager::clean()
{
	SDL_DestroyTexture(this->ribTexture);	// deletes texture which was allocated in the init function
	SDL_DestroyRenderer(this->renderer);
}

int ProjectileManager::getMouseX()
{
	return mouseX;
}

int ProjectileManager::getMouseY()
{
	return mouseY;
}