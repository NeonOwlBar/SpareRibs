#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "TiledMap.h"

#define PI 3.14159265

enum KeyStates
{	//	values increment by 1, so RIGHT = 1, UP = 2...
	LEFT = 0,	// LEFT = 0 by default, does not *need* to be defined.
	RIGHT,
	UP,
	DOWN,
	SHOOT,
	INTERACT,
	KEYSTATES_COUNT
};

// for changing srcRect depending on ammo/rib count
const int activeSprite[] =
{
	0, 1, 2, 3, 4, 5, 6
};

class Player
{
public:
	Player(SDL_Renderer* sdlRenderer, float _windowWidth, float _windowHeight, std::string filename); 
	virtual ~Player() { clean(); };	// destructor

	// Array stores key states, making them accessible as bools
	// Each key is set to true if pressed.
	bool keyStates[KeyStates::KEYSTATES_COUNT];

	void init();
	void render();
	void update();
	void clean();
	
	void walkingAnim();
	int setAnimFrame();
	void setPosition(float x, float y);
	void setAngle(int x, int y);
	void setAmmo(int change);
	void setHealth(int change);
	void setScore(int score);

	float getX();
	float getY();
	float getWidth();
	float getHeight();
	float getSpeed();	// allows speed to be read-only by other classes
	float getAngle(); //const;	// makes angle accessible to other classes
	int getAmmo();
	int getHealth();
	int getScore();
	std::string getScoreStr();

	TiledMap* mapPlayer;
private:
	Uint32 startTime = 0;
	Uint32 currentTime = 0;
	int elapsedTime = 0;
	std::string timeStr;

	float speed = 3;	// player speed
	float rotationAngle;	// for shooting in direction of mouse
	int ammoLevel = 6;
	int maxHP = 100;
	int currentHP = 100;
	int score = 0;
	std::string scoreStr = std::to_string(score);	// for displaying score in UI

	float windowWidth = 0;
	float windowHeight = 0;

	float playerWidth;
	float playerHeight;
	const int srcPlayerWidth = 80;
	const int srcPlayerHeight = 155;

	// score UI
	SDL_Color colourUI = { 255, 255, 255, 255 };	// sets colour of score UI
	SDL_Rect dstScoreTxt{ 55, 0, 100, 50 };			// sets co-ordinates and size of "Score: " in UI
	SDL_Rect dstScoreNum{ (dstScoreTxt.x + dstScoreTxt.w), dstScoreTxt.y, 40, dstScoreTxt.h };	// sets co-ordinates and size of score value in UI
	SDL_Rect dstTimerTxt{ 605, 0, 100, 50 };			// sets co-ordinates and size of "Score: " in UI
	SDL_Rect dstTimerNum{ (dstTimerTxt.x + dstTimerTxt.w), dstTimerTxt.y, 40, dstTimerTxt.h };	// sets co-ordinates and size of score value in UI

	SDL_Surface* scoreText = nullptr;
	SDL_Texture* scoreTxtTexture = nullptr;
	SDL_Surface* scoreNum = nullptr;
	SDL_Texture* scoreNumTexture = nullptr;
	SDL_Surface* timerText = nullptr;
	SDL_Texture* timerTxtTexture = nullptr;
	SDL_Surface* timerNum = nullptr;
	SDL_Texture* timerNumTexture = nullptr;

	std::string playerFile;

	SDL_Rect srcRect;	// area of source file displayed
	SDL_Rect dstRect;	// area of screen where srcRect is rendered
	SDL_Rect healthBackground;
	SDL_Rect healthBar;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
	TTF_Font* fontUI = nullptr;

	int mouseX, mouseY;	// for player sprite looking in direction of mouse
	int animFrame = 0;	// sets "height" of srcRect during animation loop. 0 is idle.
	int ANIMATION_SPEED = 200; // number of ms between frames
	long int lastAnimChange = 0;
};

