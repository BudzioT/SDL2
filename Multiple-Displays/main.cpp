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

/* Global window */
LWindow window;

/* Display data */
int totalDisplays = 0;
SDL_Rect* displayBounds = nullptr;


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

	/* Initialize the window */
	window.init();


	bool quit = false;
	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			/* Handle window events */
			window.handleEvent(e);
		}

		/* Update all windows */
		window.render();
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

	/* Set texture filtering to linear */
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		printf("Warning: Linear texture filtering not enabled!\n");

	/* Get number of displays */
	totalDisplays = SDL_GetNumVideoDisplays();
	if (totalDisplays < 2)
		printf("Warning: Only one display connected!");

	/* Get bounds of each display */
	displayBounds = new SDL_Rect[totalDisplays];
	for (int i = 0; i < totalDisplays; ++i)
		SDL_GetDisplayBounds(i, &displayBounds[i]);

	/* Initialize window */
	if (!window.init()) {
		printf("Couldn't initialize window! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

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
	window.free();

	/* Free the memory */
	delete[] displayBounds;

	/* Quit the SDL library */
	SDL_Quit();
}