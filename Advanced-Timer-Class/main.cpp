#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "LTexture.h"
#include "LTimer.h"

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

/* Text texture */
LTexture startPromptTexture;
LTexture pausePromptTexture;
LTexture timeTexture;

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

	/* Application timer */
	LTimer timer;

	/* In memory text stream */
	std::stringstream timeText;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			else if (e.type == SDL_KEYDOWN) {
				/* Start / Stop */
				if (e.key.keysym.sym == SDLK_s) {
					if (timer.isStarted())
						timer.stop();
					else
						timer.start();
				}

				/* Pause / Unpause */
				if (e.key.keysym.sym == SDLK_p) {
					if (timer.isPaused())
						timer.unpause();
					else
						timer.pause();
				}
			}
		}

		/* Set text to be rendered */
		timeText.str("");
		timeText << "Seconds since start time " << (timer.getTicks() / 1000.0f);

		/* Load timer's text */
		if (!timeTexture.loadFromRenderedText(timeText.str(), textColor))
			printf("Unable to render time texture!\n");

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		
		/* Render textures */
		startPromptTexture.render((SCREEN_WIDTH - startPromptTexture.width()) / 2, 0);
		pausePromptTexture.render((SCREEN_WIDTH - pausePromptTexture.width()) / 2, startPromptTexture.height());
		timeTexture.render((SCREEN_WIDTH - timeTexture.width()) / 2, 
			(SCREEN_HEIGHT - timeTexture.height()) / 2);

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

		/* Load prompt texts */
		if (!startPromptTexture.loadFromRenderedText("Press S to Start or Stop the Timer", textColor)) {
			printf("Couldn't load start prompt texture!\n");
			success = false;
		}
		if (!pausePromptTexture.loadFromRenderedText("Press P to Pause or Unpause the Timer", textColor)) {
			printf("Couldn't load pause prompt texture!\n");
			success = false;
		}
	}

	return success;
}

void close()
{
	/* Free textures */
	timeTexture.free();
	startPromptTexture.free();
	pausePromptTexture.free();

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