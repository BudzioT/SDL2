#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "LTexture.h"

#include <stdio.h>
#include <string>


/* Initialize the program */
bool init();
/* Load all the needed media */
bool loadMedia();
/* Clean up */
void close();


/* Screen constants */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/* Global window and renderer */
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

/* Textures */
LTexture modulatedTexture;
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

	/* Modulation component */
	Uint8 a = 255;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			/* Handle key presses */
			if (e.type == SDL_KEYDOWN) {
				/* Increase alpha on 'w' */
				if (e.key.keysym.sym == SDLK_w) {
					if (a + 32 > 255)
						a = 255;
					else
						a += 32;

				}

				/* Decrase alpha on 's' */
				if (e.key.keysym.sym == SDLK_s) {
					if (a - 32 < 0)
						a = 0;
					else
						a -= 32;
				}
			}

		}

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* Render background texture */
		backgroundTexture.render(0, 0);

		/* Modulate and render front texture */
		modulatedTexture.setAlpha(a);
		modulatedTexture.render(0, 0);

		/* Update screen */
		SDL_RenderPresent(renderer);
	}
	
	/* Clean up */
	close();
	return 0;
}


bool init()
{
	/* Initialize video */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL couldn't initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	/* Create window */
	window = SDL_CreateWindow("Key Presses optimized", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window couldn't be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	/* Create renderer for window */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("Renderer couldn't be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	/* Initialize renderer color */
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	/* Initialize PNG load */
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image coudln't initialize! SDL_image_Error: %s\n", IMG_GetError());
		return false;
	}

	return true;
}

bool loadMedia()
{
	bool success = true;

	/* Load front texture */
	if (!modulatedTexture.loadFromFile("Images/fadeout.png")) {
		printf("Couldn't load front texture!\n");
		success = false;
	}
	else {
		/* Set standard alpha blending */
		modulatedTexture.setBlendMode(SDL_BLENDMODE_BLEND);
	}

	/* Load background texture */
	if (!backgroundTexture.loadFromFile("Images/fadein.png")) {
		printf("Couldn't load background texture!\n");
		success = false;
	}

	return success;
}

void close()
{
	/* Free loaded textures */
	modulatedTexture.free();

	/* Destroy window */
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	/* Quit the SDL library */
	IMG_Quit();
	SDL_Quit();
}