#include "SDL2/SDL.h"

#include "LWindow.h"

#include <stdio.h>
#include <string>


/* Initialize the program */
bool init();
/* Load all the needed media */
bool loadMedia();
/* Clean up */
void close();


/* Screen dimensions */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/* Global window and renderer */
LWindow windows[TOTAL_WINDOWS];


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

	/* Initialize the rest of the windows */
	for (int i = 1; i < TOTAL_WINDOWS; i++)
		windows[i].init();


	bool quit = false;
	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			/* Handle window events */
			for (int i = 0; i < TOTAL_WINDOWS; ++i)
				windows[i].handleEvent(e);

			/* Pull up window */
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_1:
					windows[0].focus();
					break;

				case SDLK_2:
					windows[1].focus();
					break;

				case SDLK_3:
					windows[2].focus();
					break;
				}
			}
		}

		/* Update all windows */
		for (int i = 0; i < TOTAL_WINDOWS; ++i)
			windows[i].render();
		
		/* Check all windows */
		bool allWindowsClosed = true;
		for (int i = 0; i < TOTAL_WINDOWS; ++i) {
			if (windows[i].isShown()) {
				allWindowsClosed = false;
				break;
			}
		}

		/* Application closed all windows */
		if (allWindowsClosed)
			quit = true;
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

	/* Initialize window */
	if (!windows[0].init()) {
		printf("Couldn't initialize window 0! SDL_Error: %s\n", SDL_GetError());
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

	return success;
}

void close()
{

	/* Destroy windows */
	for (int i = 0; i < TOTAL_WINDOWS; i++)
		windows[i].free();

	/* Quit the SDL library */
	SDL_Quit();
}