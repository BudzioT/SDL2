#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "LTexture.h"
#include "Dot.h"

#include <stdio.h>
#include <string>
#include <fstream>


/* Initialize the program */
bool init();
/* Load all the needed media */
bool loadMedia(Tile* tiles[]);
/* Clean up */
void close(Tile* tiles[]);

/* Set tiles from tile map */
bool setTiles(Tile* tiles[]);


/* Screen dimensions */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/* Level dimensions */
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

/* Global window and renderer */
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

/* Particle textures */
LTexture redTexture;
LTexture greenTexture;
LTexture blueTexture;
LTexture shimmerTexture;

/* Tile texture and clips */
LTexture tileTexture;
SDL_Rect tileClips[TOTAL_TILE_SPRITES];


int main(int argc, char* args[])
{
	/* The level tiles */
	Tile* tileSet[TOTAL_TILES];

	/* Initialize */
	if (!init()) {
		printf("Failed to initialize!\n");
		close(tileSet);
		return -1;
	}

	/* Load media */
	if (!loadMedia(tileSet)) {
		printf("Failed to load media!\n");
		close(tileSet);
		return -1;
	}


	bool quit = false;
	SDL_Event e;

	/* Dot that will be moving around the screen */
	Dot dot;
	/* Camera that follows dot */
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			/* Handle dot events */
			dot.handleEvent(e);
		}

		/* Move the dot */
		dot.move(tileSet);
		dot.setCamera(camera);

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* Render level */
		for (int i = 0; i < TOTAL_TILES; ++i)
			tileSet[i]->render(camera);

		/* Render objects */
		dot.render(camera);

		/* Update screen */
		SDL_RenderPresent(renderer);
	}

	/* Clean up */
	close(tileSet);
	return 0;
}


bool init()
{
	bool success = true;

	/* Initialize video */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL couldn't initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	/* Set texture filtering to linear */
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		printf("Warning: Linear texture filtering not enabled!\n");

	/* Create window */
	window = SDL_CreateWindow("SDL Particle Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		printf("Couldn't create window! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		/* Create renderer */
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!renderer) {
			printf("Couldn't create renderer! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
	}

	/* Initialize PNG loading */
	int IMG_FLAGS = IMG_INIT_PNG;
	if (!(IMG_Init(IMG_FLAGS) & IMG_FLAGS)) {
		printf("SDL_img couldn't initialize! IMG_Error: %s\n", IMG_GetError());
		success = false;
	}

	return success;
}

bool loadMedia(Tile* tiles[])
{
	bool success = true;

	/* Load particle textures */
	if (!redTexture.loadFromFile("Images/red.bmp")) {
		printf("Couldn't load red particle texture!\n");
		success = false;
	}
	if (!greenTexture.loadFromFile("Images/green.bmp")) {
		printf("Couldn't load green particle texture!\n");
		success = false;
	}
	if (!blueTexture.loadFromFile("Images/blue.bmp")) {
		printf("Couldn't load blue particle texture!\n");
		success = false;
	}
	if (!shimmerTexture.loadFromFile("Images/shimmer.bmp")) {
		printf("Couldn't load shimmer particle texture!\n");
		success = false;
	}
	/* Set particle textures transparency */
	redTexture.setAlpha(192);
	greenTexture.setAlpha(192);
	blueTexture.setAlpha(192);
	shimmerTexture.setAlpha(192);

	/* Load tiles texture */
	if (!tileTexture.loadFromFile("Images/tiles.png")) {
		printf("Couldn't load tiles texture!\n");
		success = false;
	}

	/* Load tile map */
	if (!setTiles(tiles)) {
		printf("Couldn't set the tiles!\n");
		success = false;
	}

	return success;
}

void close(Tile* tiles[])
{
	/* Free particle textures */
	redTexture.free();
	greenTexture.free();
	blueTexture.free();
	shimmerTexture.free();

	/* Free tile texture */
	tileTexture.free();

	/* Free tiles */
	for (int i = 0; i < TOTAL_TILES; ++i) {
		delete tiles[i];
		tiles[i] = nullptr;
	}

	/* Destroy windows */
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	/* Destroy renderer */
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	/* Quit the SDL library */
	IMG_Quit();
	SDL_Quit();
}

bool setTiles(Tile* tiles[])
{
	/* Success flag */
	bool tilesLoaded = true;

	/* Tile offsets */
	int x = 0, y = 0;

	/* Open the map */
	std::ifstream map("Images/lazy.map");

	if (map.fail()) {
		printf("Couldn't open tile map!\n");
		tilesLoaded = false;
	}
	else {
		/* Initialize the tiles */
		for (int i = 0; i < TOTAL_TILES; ++i) {
			/* Determines the type of tile */
			int tileType = -1;
			/* Read type from map file */
			map >> tileType;

			/* If there was a problem */
			if (map.fail()) {
				/* Stop loading map */
				printf("Error loading map: Unexcepted end of file!\n");
				tilesLoaded = false;
				break;
			}

			/* If type is valid */
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
				tiles[i] = new Tile(x, y, static_cast<Tiles>(tileType));
			/* If we don't recognize tile type */
			else {
				/* Stop loading map */
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			/* Move to the next tile spot */
			x += TILE_WIDTH;

			/* If gone too far */
			if (x >= LEVEL_WIDTH) {
				/* Move back */
				x = 0;

				/* Move to the next row */
				y += TILE_HEIGHT;
			}
		}

		if (tilesLoaded) {
			tileClips[static_cast<int>(Tiles::RED)] = { 0, 0, TILE_WIDTH, TILE_HEIGHT };

			tileClips[static_cast<int>(Tiles::GREEN)] = { 0, 80, TILE_WIDTH, TILE_HEIGHT };

			tileClips[static_cast<int>(Tiles::BLUE)] = { 0, 160, TILE_WIDTH, TILE_HEIGHT };

			tileClips[static_cast<int>(Tiles::TOPLEFT)] = { 80, 0, TILE_WIDTH, TILE_HEIGHT };

			tileClips[static_cast<int>(Tiles::LEFT)] = { 80, 80, TILE_WIDTH, TILE_HEIGHT };

			tileClips[static_cast<int>(Tiles::BOTTOMLEFT)] = { 80, 160, TILE_WIDTH, TILE_HEIGHT };

			tileClips[static_cast<int>(Tiles::TOP)] = { 160, 0, TILE_WIDTH, TILE_HEIGHT };

			tileClips[static_cast<int>(Tiles::CENTER)] = { 160, 80, TILE_WIDTH, TILE_HEIGHT };

			tileClips[static_cast<int>(Tiles::BOTTOM)] = { 160, 160, TILE_WIDTH, TILE_HEIGHT };

			tileClips[static_cast<int>(Tiles::TOPRIGHT)] = { 240, 0, TILE_WIDTH, TILE_HEIGHT };

			tileClips[static_cast<int>(Tiles::RIGHT)] = { 240, 80, TILE_WIDTH, TILE_HEIGHT };

			tileClips[static_cast<int>(Tiles::BOTTOMRIGHT)] = { 240, 160, TILE_WIDTH, TILE_HEIGHT };
		}
	}

	/* Close the file */
	map.close();

	return tilesLoaded;
}