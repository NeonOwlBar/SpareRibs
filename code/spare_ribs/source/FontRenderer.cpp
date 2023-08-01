#include "FontRenderer.h"

FontRenderer::FontRenderer(SDL_Renderer* sdlRenderer)
{
	renderer = sdlRenderer;
}

void FontRenderer::init()
{
	int init = TTF_Init();
	if (init != 0)
	{
		std::cout << SDL_GetError() << std::endl;
	}

	font = TTF_OpenFont("assets/OpenSans-Regular.ttf", 30);
	if (font == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
	}
}

void FontRenderer::render(std::string text)
{
	SDL_Color colour = { 120, 0, 60, 255 }; 

	// The following creates an image representing the text that we input
	SDL_Surface* textImage = TTF_RenderText_Solid(font, text.c_str(), colour);
	// Once we have an image, we can render it the same way we do with our player
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textImage);
	// RenderCopy is the same function as used for the player in Player.cpp
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	// Since we rendered our text, we don't need the texture
	// nor the image: we can now release them
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(textImage);
}

void FontRenderer::clean()
{
	// Free up all the resources we used to render
	TTF_CloseFont(font);
	font = nullptr;
	delete font;
	SDL_DestroyRenderer(renderer);
}
