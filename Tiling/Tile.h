#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "LTexture.h"


/* Tile constants */
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

/* Diffrent tile sprites */
enum class Tiles {
	RED,
	GREEN,
	BLUE,
	CENTER,
	TOP,
	TOPRIGHT,
	RIGHT,
	BOTTOMRIGHT,
	BOTTOM,
	BOTTOMLEFT,
	LEFT,
	TOPLEFT
};

/* Global tile texture and clips */
extern LTexture tileTexture;
extern SDL_Rect tileClips[TOTAL_TILE_SPRITES];


/* Tile */
class Tile
{
public:
	/* Initialize position and type */
	Tile(int x, int y, Tiles type);

	/* Show the tile */
	void render(SDL_Rect& camera);

	/* Get tile type */
	Tiles getType();
	/* Get collision box */
	SDL_Rect getBox();

private:
	/* Attributes of a tile */
	SDL_Rect m_box;

	/* Tile type */
	Tiles m_type;
};


/* Box collision detector */
bool checkCollision(SDL_Rect a, SDL_Rect b);

/* Check collision box against set of tiles */
bool touchesWall(SDL_Rect box, Tile* tiles[]);