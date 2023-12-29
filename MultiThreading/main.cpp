#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_thread.h"

#include "LTexture.h"

#include <stdio.h>
#include <string>
#include <fstream>


/* Initialize the program */
bool init();
/* Load all the needed media */
bool loadMedia();
/* Clean up */
void close();

/* Test thread function */
int threadFunction(void* data);


/* Screen dimensions */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/* Global window and renderer */
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

/* Texture */
LTexture splashTexture;


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

	/* Run the thread */
	int data = 101;
	SDL_Thread* threadID = SDL_CreateThread(threadFunction, "SomeThread", reinterpret_cast<void*>(data));

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
		}

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* Render splash texture */
		splashTexture.render(0, 0);

		/* Update screen */
		SDL_RenderPresent(renderer);
	}

	/* Wait for thread to finish */
	SDL_WaitThread(threadID, NULL);

	/* Clean up */
	close();
	return 0;
}


bool init()
{
	bool success = true;

	/* Initialize video */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		printf("SDL couldn't initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	/* Set texture filtering to linear */
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		printf("Warning: Linear texture filtering not enabled!\n");

	/* Create window */
	window = SDL_CreateWindow("SDL Particle Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		printf("Couldn't create window! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		/* Create renderer */
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!renderer) {
			printf("Couldn't create renderer! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
	}

	/* Initialize PNG loading */
	int IMG_FLAGS = IMG_INIT_PNG;
	if (!(IMG_Init(IMG_FLAGS) & IMG_FLAGS)) {
		printf("SDL_img couldn't initialize! IMG_Error: %s\n", IMG_GetError());
		success = false;
	}

	return success;
}

bool loadMedia()
{
	bool success = true;

	if (!splashTexture.loadFromFile("Images/splash.png")) {
		printf("Couldn't load splash texture!\n");
		success = false;
	}

	return success;
}

void close()
{
	/* Free texture */
	splashTexture.free();

	/* Destroy windows */
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	/* Destroy renderer */
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	/* Quit the SDL library */
	IMG_Quit();
	SDL_Quit();
}

int threadFunction(void* data)
{
	/* Print incoming data */
	printf("Running thread with value = %d\n", reinterpret_cast<int>(data));

	return 0;
}