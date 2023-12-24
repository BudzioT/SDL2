#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "LTexture.h"

#include <stdio.h>
#include <string>
#include <sstream>


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
TTF_Font* font = nullptr;

/* Prompt texture */
LTexture promptTexture;
LTexture timeTextTexture;

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

	/* Set text color as black */
	SDL_Color textColor = { 0, 0, 0, 255 };

	/* Current time start time */
	Uint32 startTime = 0;

	/* In memory text stream */
	std::stringstream timeText;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			/* Reset start time on return keypress */
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
				startTime = SDL_GetTicks();
		}

		/* Set text to be rendered */
		timeText.str("");
		timeText << "Miliseconds since start time " << SDL_GetTicks() - startTime;

		/* Render text */
		if (!timeTextTexture.loadFromRenderedText(timeText.str(), textColor))
			printf("Unable to render time texture!\n");

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		
		/* Render textures */
		promptTexture.render((SCREEN_WIDTH - promptTexture.width()) / 2, 0);
		timeTextTexture.render((SCREEN_WIDTH - promptTexture.width()) / 2,
			(SCREEN_HEIGHT - promptTexture.height()) / 2);

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

	/* Initialize TTF */
	if (TTF_Init() == -1) {
		printf("SDL_TTF coudln't initialize! TTF_Error: %s\n", TTF_GetError());
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

	/* Open the font */
	font = TTF_OpenFont("Fonts/lazy.ttf", 28);
	if (font == nullptr) {
		printf("Couldn't load lazy font! TTF_Error: %s\n", TTF_GetError());
		success = false;
	}
	else {
		/* Set text color as black */
		SDL_Color textColor = { 0, 0, 0, 255 };

		/* Load prompt text */
		if (!promptTexture.loadFromRenderedText("Press Enter to Reset Start Time.", textColor)) {
			printf("Unable to render prompt texture!\n");
			success = false;
		}
	}

	return success;
}

void close()
{
	/* Free textures */
	promptTexture.free();
	timeTextTexture.free();

	/* Close the font */
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