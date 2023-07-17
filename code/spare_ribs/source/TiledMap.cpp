#include "TiledMap.h"
#include <fstream>
#include <iostream>

TiledMap::TiledMap(SDL_Renderer* sdlRenderer, std::string tiledMapFilename)
{
	renderer = sdlRenderer;
	filename = tiledMapFilename;
}

int TiledMap::init()
{
	SDL_Surface* image = IMG_Load(filename.c_str());
	texture = SDL_CreateTextureFromSurface(renderer, image);

	// tile map visuals
	
	std::ifstream lvl1Data;
	lvl1Data.open("assets/level1.txt");
	if (!lvl1Data)
	{
		std::cout << "Could not find level 1 tile map." << std::endl;
	}

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			int tile;
			lvl1Data >> tile;
			mapData[i][j] = tile;
		}
	}
	lvl1Data.close();


	// tile map collisions
	std::ifstream lvl1Collisions;
	lvl1Collisions.open("assets/lvl1Collisions.txt");
	if (!lvl1Collisions)
	{
		std::cout << "Could not find level 1 collision data" << std::endl;
	}

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			int tileColl;
			lvl1Collisions >> tileColl;
			collisionLvl1[i][j] = tileColl;
		}
	}
	lvl1Collisions.close();

	SDL_FreeSurface(image);
	return 0;
}

void TiledMap::render()
{
	for (int i = 0; i < 12; i++)
	{	// nested for loops cycle each entry in the custom tiled map
		for (int j = 0; j < 16; j++)
		{
			// crop the corresponding tile from source img, using sourceRect
			SDL_Rect sourceRect;
			sourceRect.x = (mapData[i][j] % 10) * sourceTileSizePx;
			sourceRect.y = (mapData[i][j] / 10) * sourceTileSizePx;
			sourceRect.h = sourceTileSizePx;
			sourceRect.w = sourceTileSizePx;

			// composes tiles for each iteration using specified dimensions, using renderRect
			SDL_Rect renderRect;
			renderRect.x = j * tileWidth;
			renderRect.y = i * tileHeight;
			renderRect.h = tileHeight;
			renderRect.w = tileWidth;

			SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
		}
	}
}

void TiledMap::clean()
{
	SDL_DestroyTexture(this->texture);	// deletes texture which was allocated in the init function
}