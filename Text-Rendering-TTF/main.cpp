#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "LTexture.h"

#include <stdio.h>
#include <string>
#include <cmath>


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

/* Global font */
TTF_Font* font = NULL;

/* Textures */
LTexture textTexture;


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

		/* Render current frame */
		textTexture.render((SCREEN_WIDTH - textTexture.width()) / 2,
			(SCREEN_HEIGHT - textTexture.height()) / 2);
		
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

	/* Initialize SDL_ttf */
	if (TTF_Init() == -1) {
		printf("SDL_TTF coudln't initalize! TTF_Error: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

bool loadMedia()
{
	bool success = true;

	/* Open the font */
	font = TTF_OpenFont("Fonts/lazy.ttf", 28);
	if (font == NULL) {
		printf("Failed to load lazy font! TTF_Error: %s!", TTF_GetError());
		success = false;
	}
	else {
		/* Render text */
		SDL_Color textColor = { 0, 0, 0 };
		if (!textTexture.loadFromRenderedText("The quick brown fox jumps over the lazy dog", textColor)) {
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	return success;
}

void close()
{
	/* Free loaded textures */
	textTexture.free();

	/* Free global font */
	TTF_CloseFont(font);
	font = nullptr;

	/* Destroy window */
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	/* Quit the SDL library */
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}