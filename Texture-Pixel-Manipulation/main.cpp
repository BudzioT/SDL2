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
bool loadMedia();
/* Clean up */
void close();


/* Screen dimensions */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/* Global window and renderer */
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

/* Texture */
LTexture guyTexture;


int main(int argc, char* args[])
{
	/* Initialize */
	if (!init()) {
		printf("Failed to initialize!\n");
		close();
		return -1;
	}

	/* Load media */
	if (!loadMedia()) {
		printf("Failed to load media!\n");
		close();
		return -1;
	}


	bool quit = false;
	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
		}

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* Render the texture */
		guyTexture.render((SCREEN_WIDTH - guyTexture.width()) / 2, (SCREEN_HEIGHT - guyTexture.height()) / 2);

		/* Update screen */
		SDL_RenderPresent(renderer);
	}

	/* Clean up */
	close();
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

bool loadMedia()
{
	bool success = true;

	/* Load guy texture */
	if (!guyTexture.loadPixelsFromFile("Images/guy.png")) {
		printf("Couldn't load guy texture!\n");
		success = false;
	}
	else {
		/* Get pixel data */
		Uint32* pixels = guyTexture.getPixels32();
		int pixelCount = guyTexture.getPitch32() * guyTexture.height();

		/* Map colors */
		Uint32 colorKey = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0xFF, 0x00, 0xFF, 0xFF);
		Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(window)->format, 0xFF, 0xFF, 0xFF, 0x00);

		/* Color key pixels */
		for (int i = 0; i < pixelCount; ++i) {
			if (pixels[i] == colorKey)
				pixels[i] = transparent;
		}

		/* Create texture from manually color keyed pixels */
		if (!guyTexture.loadFromPixels())
			printf("Couldn't load guy texture from surface!\n");
	}

	return success;
}

void close()
{
	/* Free texture */
	guyTexture.free();

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