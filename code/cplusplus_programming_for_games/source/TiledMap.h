#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>

class TiledMap
{
public:

	TiledMap(SDL_Renderer* sdlRenderer, std::string tiledMapFilename);
	// defining the tiled map with a 2D array
	int mapData[12][16];	// each int in this array refers to a tile in the image


	int collisionLvl1[12][16];
	// each int in this array refers to a tile in the image
	// 0 represents open space, or a wall that does not 
	// need to be checked for collision.

	// 1 represents a wall. There was a glitch a zombie 
	// could clip through walls if it collided with more 
	// than one wall ("wall" according to this array).
	// Therefore, the walls are placed in a way that a zombie
	// will never touch more than one "wall" in  any given frame.

	int init();
	void render();
	void clean();

private:
	const int sourceTileSizePx = 16;	// defines size of tile in the source image
	const int tileWidth = 50;	// defines dimensions of tile inside the level
	const int tileHeight = 50;	

	std::string filename;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

};

