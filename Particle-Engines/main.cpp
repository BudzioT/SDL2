#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "LTexture.h"
#include "Dot.h"

#include <stdio.h>
#include <string>


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

	/* Dot that will be moving around the screen */
	Dot dot;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			/* Handle dot events */
			dot.handleEvent(e);
		}

		/* Move the dot */
		dot.move();

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* Render objects */
		dot.render();

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

	return success;
}

void close()
{
	/* Free textures */
	redTexture.free();
	greenTexture.free();
	blueTexture.free();

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