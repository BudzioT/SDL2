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

/* Worker functions */
int producer(void* data);
int consumer(void* data);
void produce();
void consume();


/* Screen dimensions */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/* Global window and renderer */
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

/* Texture */
LTexture splashTexture;

/* Protetctive mutex */
SDL_mutex* bufferLock = nullptr;

/* Conditions */
SDL_cond* canProduce = nullptr;
SDL_cond* canConsume = nullptr;

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
	SDL_Thread* threadA = SDL_CreateThread(producer, "Thread A", (void*)"Thread A");
	SDL_Delay(16 + rand() % 32);
	SDL_Thread* threadB = SDL_CreateThread(consumer, "Thread B", (void*)"Thread B");

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

	/* Create mutex */
	bufferLock = SDL_CreateMutex();

	/* Create conditions */
	canProduce = SDL_CreateCond();
	canConsume = SDL_CreateCond();

	/* Load splash texture */
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

	/* Destroy mutex */
	SDL_DestroyMutex(bufferLock);
	bufferLock = nullptr;

	/* Destroy conditions */
	SDL_DestroyCond(canProduce);
	SDL_DestroyCond(canConsume);
	canProduce = nullptr;
	canConsume = nullptr;

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

int producer(void* data)
{
	printf("\nProducer started...\n");

	/* Seed thread random */
	srand(SDL_GetTicks());

	for (int i = 0; i < 5; ++i) {
		/* Wait */
		SDL_Delay(rand() % 1000);

		/* Produce */
		produce();
	}

	printf("\nProducer finished!\n");

	return 0;
}

int consumer(void* data)
{
	printf("\nConsumer started...\n");

	/* Seed thread random */
	srand(SDL_GetTicks());

	for (int i = 0; i < 5; ++i) {
		/* Wait */
		SDL_Delay(rand() % 1000);

		/* Consume */
		consume();
	}

	printf("\nConsumer finished!\n");

	return 0;
}

void produce()
{
	/* Lock */
	SDL_LockMutex(bufferLock);

	/* If the buffer is full */
	if (globalData != -1) {
		/* Wait for the buffer to be cleared */
		printf("\nProducer encountered full buffer, waiting for consumer to empty buffer...\n");
		SDL_CondWait(canProduce, bufferLock);
	}

	/* Fill and show buffer */
	globalData = rand() % 255;
	printf("\nProduced %d\n", globalData);

	/* Unlock */
	SDL_UnlockMutex(bufferLock);

	/* Signal consumer */
	SDL_CondSignal(canConsume);
}

void consume()
{
	/* Lock */
	SDL_LockMutex(bufferLock);

	/* If buffer is empty */
	if (globalData == -1) {
		/* Wait for the buffer to be filled */
		printf("\nConsumer encountered empty buffer, waiting for producer to fill buffer...\n");
		SDL_CondWait(canConsume, bufferLock);
	}

	/* Show and empty buffer */
	printf("\nConsumed %d\n", globalData);
	globalData = -1;

	/* Unlock */
	SDL_UnlockMutex(bufferLock);

	/* Signal producer */
	SDL_CondSignal(canProduce);
}