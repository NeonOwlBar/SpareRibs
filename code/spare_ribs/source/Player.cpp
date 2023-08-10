#include "Player.h"

Player::Player(SDL_Renderer* sdlRenderer, float _windowWidth, float _windowHeight, std::string filename)
{
	renderer = sdlRenderer;
	windowWidth = _windowWidth;
	windowHeight = _windowHeight;
	playerFile = filename;
}

void Player::init()
{
	// c_str returns a pointer to an array that contains a null-terminated sequence of characters 
	// representing the current value of the string object.
	SDL_Surface* image = IMG_Load(playerFile.c_str());	// 352 x 727 pixels
	if (image == NULL)
	{
		std::cout << "Could not load player sprite!" << std::endl;
		return;
	}
	texture = SDL_CreateTextureFromSurface(renderer, image);
	if (texture == NULL)
	{
		std::cout << "Could not create player texture" << std::endl;
		return;
	}

	setPosition(100, 350);

	playerWidth = 0.075f * windowWidth;	// Player will always be x % of window width
	playerHeight = 2 * playerWidth;	// declares playerHeight, defined as 2 * playerWidth so it is always in correct ratio
	dstRect.w = playerWidth;
	dstRect.h = playerHeight;
	
	// rendering text for score
	int initUI = TTF_Init();
	if (initUI != 0)
	{
		std::cout << SDL_GetError() << std::endl;
	}


	fontUI = TTF_OpenFont("assets/OpenSans-Regular.ttf", 16);
	if (fontUI == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
	}
	
	lastAnimChange = SDL_GetTicks();
	startTime = SDL_GetTicks();
}

void Player::render()
{
	currentTime = SDL_GetTicks();
	elapsedTime = (currentTime - startTime) / 1000;
	timeStr = std::to_string(elapsedTime);	// updates value to current time

	animX = activeSprite[ammoLevel] * srcPlayerWidth;
	animY = animFrame;
	srcRect = { animX, animY, srcPlayerWidth, srcPlayerHeight };

	SDL_GetMouseState(&mouseX, &mouseY);
	// rib astley faces left or right, depending on where cursor is to his left or right
	if (mouseX >= (dstRect.x + (playerWidth / 2)))
	{
		SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
	} 
	else 
	{
		SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0, NULL, SDL_FLIP_HORIZONTAL);
	}
	
	// creating a health bar
	pixelWidth = windowWidth / 4;	// takes up 1/4 of the screen's width
	unitFactor = (float)pixelWidth / (float)maxHP;	//	100HP total, unit factor represents 1HP
	hpWidth = currentHP * unitFactor;
	
	healthBackground = { 275, 12, pixelWidth, 25 };	// red background bar
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &healthBackground);
	healthBar = { 275, 12, hpWidth, 25 };	// green bar indicating health value
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &healthBar);

	
	// rendering score 
	// Creates an image representing the text in the second parameter
	scoreText = TTF_RenderText_Solid(fontUI, "Score: ", colourUI);
	// Renders textImage
	scoreTxtTexture = SDL_CreateTextureFromSurface(renderer, scoreText);
	// RenderCopy is the same function as used for the player in Player.cpp
	SDL_RenderCopy(renderer, scoreTxtTexture, NULL, &dstScoreTxt);

	// Creates an image representing the text in the second parameter
	scoreNum = TTF_RenderText_Solid(fontUI, scoreStr.c_str(), colourUI);
	// Renders textImage
	scoreNumTexture = SDL_CreateTextureFromSurface(renderer, scoreNum);
	// RenderCopy is the same function as used for the player in Player.cpp
	SDL_RenderCopy(renderer, scoreNumTexture, NULL, &dstScoreNum);


	// rendering timer 
	// Creates an image representing the text in the second parameter
	timerText = TTF_RenderText_Solid(fontUI, "Time: ", colourUI);
	// Renders textImage
	timerTxtTexture = SDL_CreateTextureFromSurface(renderer, timerText);
	// RenderCopy is the same function as used for the player in Player.cpp
	SDL_RenderCopy(renderer, timerTxtTexture, NULL, &dstTimerTxt);

	// Creates an image representing the text in the second parameter
	timerNum = TTF_RenderText_Solid(fontUI, timeStr.c_str(), colourUI);
	// Renders textImage
	timerNumTexture = SDL_CreateTextureFromSurface(renderer, timerNum);
	// RenderCopy is the same function as used for the player in Player.cpp
	SDL_RenderCopy(renderer, timerNumTexture, NULL, &dstTimerNum);

}

void Player::update()
{
	oldX = getX();
	oldY = getY();

	// Release UI surface and texture memory. 
	// This is in the update function as it greatly decreases the number of CPU calls.
	SDL_FreeSurface(scoreText);
	SDL_FreeSurface(scoreNum);
	SDL_DestroyTexture(scoreTxtTexture);
	SDL_DestroyTexture(scoreNumTexture);
	SDL_FreeSurface(timerText);
	SDL_FreeSurface(timerNum);
	SDL_DestroyTexture(timerTxtTexture);
	SDL_DestroyTexture(timerNumTexture);

	if (keyStates[LEFT])
	{
		setPosition(getX() - getSpeed(), getY());
		walkingAnim();
	} else if (keyStates[RIGHT])
	{
		setPosition(getX() + getSpeed(), getY());
		walkingAnim();
	}
	if (keyStates[UP])
	{
		setPosition(getX(), getY() - getSpeed());
		walkingAnim();
	} else if (keyStates[DOWN])
	{
		setPosition(getX(), getY() + getSpeed());
		walkingAnim();
	}

	newX = getX();
	newY = getY();
	playerPos = { newX, (int)(newY + (playerHeight / 3)), playerWidth, playerHeight * 2 / 3 };
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (mapPlayer->collisionLvl1[i][j] == 1)
			{
				mapTile = { j * 50, i * 50, 50, 50 };

				if (SDL_HasIntersection(&playerPos, &mapTile))
				{
					setPosition(oldX, oldY);

					// attempt to allow player to continue moving if player holding 
					// two directional buttons, when one direction is colliding

					// changes need to be made to check which part of player rect is colliding
					// with map tile, allowing other direction to still be allowed.

					/*if (newX < (mapTile.x + mapTile.w) || (newX + playerWidth) > mapTile.x) {
						newX = oldX;
					}
					if (newY > (mapTile.y + mapTile.h) || (newY + playerHeight) > mapTile.y) {
						newY = oldY;
					}

					setPosition(newX, newY);*/
				}
			}
		}
	}
}

void Player::clean()
{
	mapPlayer = nullptr;
	scoreText = nullptr;
	scoreTxtTexture = nullptr;
	scoreNum = nullptr;
	scoreNumTexture = nullptr;
	timerText = nullptr;
	timerTxtTexture = nullptr;
	timerNum = nullptr;
	timerNumTexture = nullptr;
	renderer = nullptr;
	texture = nullptr;
	fontUI = nullptr;

	delete mapPlayer;
	delete scoreText;
	delete scoreTxtTexture;
	delete scoreNum;
	delete scoreNumTexture;
	delete timerText;
	delete timerTxtTexture;
	delete timerNum;
	delete timerNumTexture;
	delete fontUI;

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
}


void Player::walkingAnim()
{
	if (SDL_GetTicks() - lastAnimChange < ANIMATION_SPEED)
	{
		return;
	}
	const int TILESHEET_HEIGHT = 4;
	lastAnimChange = SDL_GetTicks();
	animFrame += srcPlayerHeight;
	if (animFrame >= (srcPlayerHeight * TILESHEET_HEIGHT))
	{
		animFrame = 0;
	}
}

int Player::setAnimFrame()
{
	return animFrame = 0;
}

void Player::setPosition(int x, int y)
{
	dstRect.x = x;
	dstRect.y = y;
}

void Player::setThrowAngle(int x, int y)
{
	throwAngle = (atan2((getY() + (getHeight() / 2) - y), (getX() - x)) * 180 / PI) - 90;
}

void Player::setSpinAngle(int x, int y)
{
	spinAngle = (atan2((getY() + (getHeight() / 2) - y), (getX() - x)) * 180 / PI) - 90;
}

void Player::setScore(int _score)
{
	score += _score;
	scoreStr = std::to_string(score);
}

void Player::setAmmo(int change)
{
	ammoLevel += change;
}

void Player::setHealth(int change)
{
	if ((currentHP > 0) && (currentHP <= 100))
	{
		currentHP += change;
		if (currentHP >= 100)
		{
			currentHP = 100;
		}
		else if (currentHP <= 0)
		{
			currentHP = 0;
		}
	}
}

int Player::getX()
{
	return dstRect.x;
}

int Player::getY()
{
	return dstRect.y;
}

int Player::getWidth()
{
	return dstRect.w;
}

int Player::getHeight()
{
	return dstRect.h;
}

int Player::getHealth()
{
	return currentHP;
}

float Player::getSpeed()
{
	return speed;
}

float Player::getThrowAngle() //const
{
	return throwAngle;
}

int Player::getAmmo()
{
	return ammoLevel;
}

int Player::getScore()
{
	return score;
}

std::string Player::getScoreStr()
{
	return scoreStr;
}