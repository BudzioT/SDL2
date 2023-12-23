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

/* Walking animation */
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect spriteClips[WALKING_ANIMATION_FRAMES];
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

	/* Current animation frame */
	int frame = 0;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
		}

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* Render current frame */
		SDL_Rect* currentClip = &spriteClips[frame / 4];
		spriteSheetTexture.render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2,
			currentClip);

		/* Update screen */
		SDL_RenderPresent(renderer);

		/* Go to the next frame */
		++frame;

		/* Cycle animation */
		if ((frame / 4) >= WALKING_ANIMATION_FRAMES)
			frame = 0;
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
	if (!spriteSheetTexture.loadFromFile("Images/foo.png")) {
		printf("Couldn't load walking animation texture!\n");
		success = false;
	}
	else {
		/* Set sprite clips */
		spriteClips[0].x = 0;
		spriteClips[0].y = 0;
		spriteClips[0].w = 64;
		spriteClips[0].h = 205;

		spriteClips[1].x = 64;
		spriteClips[1].y = 0;
		spriteClips[1].w = 64;
		spriteClips[1].h = 205;

		spriteClips[2].x = 128;
		spriteClips[2].y = 0;
		spriteClips[2].w = 64;
		spriteClips[2].h = 205;

		spriteClips[3].x = 192;
		spriteClips[3].y = 0;
		spriteClips[3].w = 64;
		spriteClips[3].h = 205;
	}

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