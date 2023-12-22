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
SDL_Rect spriteClips[4];
LTexture spriteSheetTexture;

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

		/* Render top left sprite */
		spriteSheetTexture.render(0, 0, &spriteClips[0]);
		/* Render top right sprite */
		spriteSheetTexture.render(SCREEN_WIDTH - spriteClips[1].w, 0, &spriteClips[1]);
		/* Render bottom left sprite */
		spriteSheetTexture.render(0, SCREEN_HEIGHT - spriteClips[2].w, &spriteClips[2]);
		/* Render bottom right sprite */
		spriteSheetTexture.render(SCREEN_WIDTH - spriteClips[3].w, SCREEN_HEIGHT - spriteClips[3].h,
			&spriteClips[3]);

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

	/* Load sprite sheet texture */
	if (!spriteSheetTexture.loadFromFile("Images/dots.png")) {
		printf("Failed to load sprite sheet texture!");
		return false;
	}
	
	/* Set top left sprite */
	spriteClips[0].x = 0;
	spriteClips[0].y = 0;
	spriteClips[0].w = 100;
	spriteClips[0].h = 100;

	/* Set top right sprite */
	spriteClips[1].x = 100;
	spriteClips[1].y = 0;
	spriteClips[1].w = 100;
	spriteClips[1].h = 100;

	/* Set bottom left sprite */
	spriteClips[2].x = 0;
	spriteClips[2].y = 100;
	spriteClips[2].w = 100;
	spriteClips[2].h = 100;

	/* Set bottom right sprite */
	spriteClips[3].x = 100;
	spriteClips[3].y = 100;
	spriteClips[3].w = 100;
	spriteClips[3].h = 100;

	return success;
}

void close()
{
	/* Free loaded textures */
	spriteSheetTexture.free();

	/* Destroy window */
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	/* Quit the SDL library */
	IMG_Quit();
	SDL_Quit();
}