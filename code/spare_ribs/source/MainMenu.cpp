#include "MainMenu.h"

MainMenu::MainMenu()
{
}

int MainMenu::init(SDL_Renderer* _renderer)
{
	renderer = _renderer;

	return 0;
}

void MainMenu::menuUpdate() 
{
	menuAlive();
}

void MainMenu::menuRender() 
{
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 37, 19, 26, 255);		// tilemap background colour
	//SDL_SetRenderDrawColor(renderer, 61, 37, 59, 255);	// tilemap floor colour
	SDL_RenderFillRect(renderer, NULL);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &rectButton1);
	//std::cout << "button rect: \n x: " << rectButton1.x << "\n y: " << rectButton1.y << "\n width: " << rectButton1.w << "\n height: " << rectButton1.h << std::endl;

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rectButton2);

	
	SDL_RenderPresent(renderer);
}

bool MainMenu::menuAlive()
{
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
				if (buttonHover(rectButton1)) return false;
			}
		}
	}

	return true;
}

bool MainMenu::menuAlive(bool retValue)
{
	return retValue;
}

void MainMenu::menuClean()
{
	delete renderer;
}

bool MainMenu::buttonHover(SDL_Rect button)
{
	// stores mouse coordinates in variables
	SDL_GetMouseState(&mouseX, &mouseY);

	// checks to see if mouse is within boundaries of button's SDL_Rect
	if (((mouseX >= button.x) && (mouseX <= button.x + button.w)) &&
		((mouseY >= button.y) && (mouseY <= (button.y + button.h)))) {
		return true;
	}
	else {
		return false;
	}
}