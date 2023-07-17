#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

class MainMenu {

public:
	MainMenu();
	~MainMenu() { menuClean(); }

	int init(SDL_Renderer* renderer);
	void menuUpdate();
	void menuRender();
	bool menuAlive();
	bool menuAlive(bool retValue);

	void menuClean();

	bool buttonHover(SDL_Rect button);

private:

	int mouseX, mouseY;

	bool isMenu = true;

	SDL_Renderer* renderer = nullptr;

	/*SDL_Surface* imgButton1 = nullptr;
	SDL_Texture* texButton1 = nullptr;*/

	SDL_Rect rectButton1{50, 50, 50, 50};
	SDL_Rect rectButton2{ 50, 150, 50, 50 };
};