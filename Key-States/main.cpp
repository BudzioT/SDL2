#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

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

#if defined(SDL_TTF_MAJOR_VERSION)
/* Global font */
TTF_Font* font = NULL;
#endif

/* Textures */
LTexture defaultTexture;
LTexture upTexture;
LTexture downTexture;
LTexture leftTexture;
LTexture rightTexture;


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

	/* Current rendered texture */
	LTexture* currentTexture = nullptr;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
		}

		/* Set texture based on current keystate */
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_UP])
			currentTexture = &upTexture;
		else if (currentKeyStates[SDL_SCANCODE_DOWN])
			currentTexture = &downTexture;
		else if (currentKeyStates[SDL_SCANCODE_LEFT])
			currentTexture = &leftTexture;
		else if (currentKeyStates[SDL_SCANCODE_RIGHT])
			currentTexture = &rightTexture;
		else
			currentTexture = &defaultTexture;

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* Render texture */
		currentTexture->render(0, 0);
		
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

	if (!defaultTexture.loadFromFile("Images/default.png")) {
		printf("Couldn't load buttons texture!\n");
		success = false;
	}

	if (!upTexture.loadFromFile("Images/up.png")) {
		printf("Couldn't load buttons texture!\n");
		success = false;
	}

	if (!downTexture.loadFromFile("Images/down.png")) {
		printf("Couldn't load buttons texture!\n");
		success = false;
	}

	if (!leftTexture.loadFromFile("Images/left.png")) {
		printf("Couldn't load buttons texture!\n");
		success = false;
	}

	if (!rightTexture.loadFromFile("Images/right.png")) {
		printf("Couldn't load buttons texture!\n");
		success = false;
	}

	return success;
}

void close()
{
	/* Free loaded textures */
	defaultTexture.free();
	upTexture.free();
	downTexture.free();
	leftTexture.free();
	rightTexture.free();

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