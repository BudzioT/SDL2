#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "LTexture.h"
#include "Dot.h"

#include <stdio.h>
#include <string>
#include <sstream>


/* Initialize the program */
bool init();
/* Load all the needed media */
bool loadMedia();
/* Clean up */
void close();


/* Screen dimensions */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/* The dimensions of the level */
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

/* Global window and renderer */
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

/* Background texture */
LTexture backgroundTexture;


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

	/* Dot that will move */
	Dot dot;

	/* Camera area */
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			/* Handle input for the dot */
			dot.handleEvent(e);
		}

		/* Move the dot */
		dot.move();

		/* Center the camera over the dot */
		camera.x = (dot.getPosX() + Dot::DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
		camera.y = (dot.getPosY() + Dot::DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

		/* Keep the camera in bounds */
		if (camera.x < 0)
			camera.x = 0;
		if (camera.y < 0)
			camera.y = 0;
		if (camera.x > LEVEL_WIDTH - camera.w)
			camera.x = LEVEL_WIDTH - camera.w;
		if (camera.y > LEVEL_HEIGHT - camera.h)
			camera.y = LEVEL_HEIGHT - camera.h;


		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* Render background */
		backgroundTexture.render(0, 0, &camera);
		
		/* Render dot */
		dot.render(camera.x, camera.y);

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

	/* Create window */
	window = SDL_CreateWindow("Key Presses optimized", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window couldn't be created! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	/* Create VSync renderer for window */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf("Renderer couldn't be created! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	/* Initialize renderer color */
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	/* Initialize PNG load */
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image coudln't initialize! SDL_image_Error: %s\n", IMG_GetError());
		success = false;
	}

	/* Set texture filtering to linear */
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		printf("Warning: Linear texture filtering not enabled!\n");

	return true;
}

bool loadMedia()
{
	bool success = true;

	if (!backgroundTexture.loadFromFile("Images/background.png")) {
		printf("Couldn't load background texture!\n");
		success = false;
	}

	return success;
}

void close()
{
	/* Free textures */
	backgroundTexture.free();

	/* Destroy window */
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	/* Quit the SDL library */
	IMG_Quit();
	SDL_Quit();
} 