#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "LTexture.h"
#include "LWindow.h"

#include <stdio.h>
#include <string>
#include <sstream>


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
LWindow window;
SDL_Renderer* renderer = nullptr;

/* Scene texture */
LTexture sceneTexture;


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
			window.handleEvent(e);
		}

		if (!window.isMinimized()) {
			/* Clear screen */
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);

			/* Render textures */
			sceneTexture.render((window.getWidth() - sceneTexture.width()) / 2,
				(window.getHeight() - sceneTexture.height()) / 2);

			/* Update screen */
			SDL_RenderPresent(renderer);
		}
	}
	
	/* Clean up */
	close();
	return 0;
}


bool init()
{
	bool success = true;

	/* Initialize video */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL couldn't initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	/* Initialize window */
	if (!window.init()) {
		printf("Couldn't initialize window! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	/* Create VSync renderer for window */
	renderer = window.createRenderer();
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

	/* Set texture filtering to linear */
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		printf("Warning: Linear texture filtering not enabled!\n");

	return true;
}

bool loadMedia()
{
	bool success = true;

	if (!sceneTexture.loadFromFile("Images/scene.png")) {
		printf("Couldn't load scene texture!\n");
		success = false;
	}

	return success;
}

void close()
{
	/* Free textures */
	sceneTexture.free();

	/* Destroy window */
	window.free();
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	/* Quit the SDL library */
	IMG_Quit();
	SDL_Quit();
}