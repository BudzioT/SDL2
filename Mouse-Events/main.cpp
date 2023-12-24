#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "LTexture.h"
#include "LButton.h"

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
LTexture buttonSpriteSheet;
SDL_Rect spriteClips[static_cast<int>(LButtonSprite::TOTAL)];

/* Buttons */
LButton buttons[TOTAL_BUTTONS];


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

			/* Handle button events */
			for (int i = 0; i < TOTAL_BUTTONS; i++)
				buttons[i].handleEvent(&e);
		}

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* Render buttons */
		for (int i = 0; i < TOTAL_BUTTONS; i++)
			buttons[i].render(buttonSpriteSheet, spriteClips);
		
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

#if defined(SDL_TTF_MAJOR_VERSION)
	/* Initialize SDL_ttf */
	if (TTF_Init() == -1) {
		printf("SDL_TTF coudln't initalize! TTF_Error: %s\n", TTF_GetError());
		return false;
	}
#endif

	return true;
}

bool loadMedia()
{
	bool success = true;

	if (!buttonSpriteSheet.loadFromFile("Images/button.png")) {
		printf("Couldn't load buttons texture!\n");
		success = false;
	}
	else {
		/* Set sprites */
		for (int i = 0; i < static_cast<int>(LButtonSprite::TOTAL); i++) {
			spriteClips[i].x = 0;
			spriteClips[i].y = i * 200;
			spriteClips[i].w = BUTTON_WIDTH;
			spriteClips[i].h = BUTTON_HEIGHT;
		}

		/* Set button in corners */
		buttons[0].setPosition(0, 0);
		buttons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
		buttons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
		buttons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
	}

	return success;
}

void close()
{
	/* Free loaded textures */
	buttonSpriteSheet.free();

#if defined(SDL_TTF_MAJOR_VERSION)
	/* Free global font */
	TTF_CloseFont(font);
	font = nullptr;
#endif

	/* Destroy window */
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	/* Quit the SDL library */
#if defined(SDL_TTF_MAJOR_VERSION)
	TTF_Quit();
#endif
	IMG_Quit();
	SDL_Quit();
}