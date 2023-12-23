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
LTexture arrowTexture;


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

	/* Angle of rotation */
	double degrees = 0.0;

	/* Flip type */
	SDL_RendererFlip flipType = SDL_FLIP_NONE;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_a:
					degrees -= 60;
					break;

				case SDLK_d:
					degrees += 60;
					break;

				case SDLK_q:
					flipType = SDL_FLIP_HORIZONTAL;
					break;

				case SDLK_w:
					flipType = SDL_FLIP_NONE;
					break;

				case SDLK_e:
					flipType = SDL_FLIP_VERTICAL;
					break;
				}
			}
		}

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* Render arrow */
		arrowTexture.render((SCREEN_WIDTH - arrowTexture.width()) / 2,
			(SCREEN_HEIGHT - arrowTexture.height()) / 2, NULL, degrees, NULL, flipType);
		
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

	/* Create VSync renderer for window */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

	/* Load sprite sheet texture */
	if (!arrowTexture.loadFromFile("Images/arrow.png")) {
		printf("Couldn't load walking animation texture!\n");
		success = false;
	}

	return success;
}

void close()
{
	/* Free loaded textures */
	arrowTexture.free();

	/* Destroy window */
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	/* Quit the SDL library */
	IMG_Quit();
	SDL_Quit();
}