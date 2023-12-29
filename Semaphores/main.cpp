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

/* Worker thread function */
int worker(void* data);


/* Screen dimensions */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/* Global window and renderer */
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

/* Texture */
LTexture splashTexture;

/* Data access semaphore */
SDL_sem* dataLock = nullptr;

/* Data buffer */
int globalData = -1;


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

	/* Run threads */
	srand(SDL_GetTicks());
	SDL_Thread* threadA = SDL_CreateThread(worker, "Thread A", (void*)"Thread A");
	SDL_Delay(16 + rand() % 32);
	SDL_Thread* threadB = SDL_CreateThread(worker, "Thread B", (void*)"Thread B");

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

	/* Wait for threads to finish */
	SDL_WaitThread(threadA, NULL);
	SDL_WaitThread(threadB, NULL);

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

	/* Initialize semaphore */
	dataLock = SDL_CreateSemaphore(1);

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

	/* Free semaphore */
	SDL_DestroySemaphore(dataLock);
	dataLock = nullptr;

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

int worker(void* data)
{
	printf("%s starting...\n", reinterpret_cast<char*>(data));

	/* Pre thread random seeding */
	srand(SDL_GetTicks());

	/* Work 5 times */
	for (int i = 0; i < 5; ++i) {
		/* Wait randomly */
		SDL_Delay(16 + rand() % 32);

		/* Lock */
		SDL_SemWait(dataLock);

		/* Print pre work data */
		printf("%s gets %d\n", reinterpret_cast<char*>(data), globalData);

		/* Work */
		globalData = rand() % 256;

		/* Print post work data */
		printf("%s sets %d\n", reinterpret_cast<char*>(data), globalData);

		/* Unlock */
		SDL_SemPost(dataLock);

		/* Wait randomly */
		SDL_Delay(16 + rand() % 640);
	}

	printf("%s finished!\n\n", reinterpret_cast<char*>(data));

	return 0;
}