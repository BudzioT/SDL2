#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <stdio.h>
#include <string>


/* Initialize the program */
bool init();
/* Load all the needed media */
bool loadMedia();
/* Load texture */
SDL_Texture* loadTexture(const std::string& path);
/* Clean up */
void close();


/* Screen constants */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum class KeyPressSurfaces {
	DEFAULT,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	TOTAL
};
SDL_Window* window = nullptr;
SDL_Texture* pressTextures[static_cast<int>(KeyPressSurfaces::TOTAL)];

SDL_Renderer* renderer = nullptr;
SDL_Texture* currentTexture = nullptr;


int main(int argc, char* args[])
{
	/* Initialize */
	if (!init()) {
		printf("Failed to initialize!\n");
		close();
		return -1;
	}

	/* Load the media */
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
		}


		/* Set renderer color */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		/* Clear screen */
		SDL_RenderClear(renderer);

		/* Top left corner viewport */
		SDL_Rect topLeftViewport = { 0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		SDL_RenderSetViewport(renderer, &topLeftViewport);
		/* Render red filled rectangle */
		SDL_Rect fillRect = { SCREEN_WIDTH / 8, SCREEN_HEIGHT / 8, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 };
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(renderer, &fillRect);

		/* Top right corner viewport */
		SDL_Rect topRightViewport = { SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		SDL_RenderSetViewport(renderer, &topRightViewport);
		/* Render green outlined quad */
		SDL_Rect outlineRect = 
			{ SCREEN_WIDTH / 8, SCREEN_HEIGHT / 8, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 };
		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawRect(renderer, &outlineRect);

		/* Normal viewport */
		SDL_Rect normal = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderSetViewport(renderer, &normal);
		/* Render blue horizontal line */
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
		SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);
		/* Render vertical line of yellow dots */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
		for (int i = 0; i < SCREEN_HEIGHT; i += 4)
			SDL_RenderDrawPoint(renderer, SCREEN_WIDTH / 2, i);

		/* Update the screen */
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

	/* Create renderer for window */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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

	return true;
}

SDL_Texture* loadTexture(const std::string& path)
{
	SDL_Texture* newTexture = nullptr;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_Error: %s\n", path.c_str(), IMG_GetError());
		return newTexture;
	}

	newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (newTexture == NULL)
		printf("Unable to create texture from %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());

	SDL_FreeSurface(loadedSurface);

	return newTexture;
}

bool loadMedia()
{
	bool success = true;
	return success;
}

void close()
{
	/* Destroy textures */
	for (int i = 0; i < static_cast<int>(KeyPressSurfaces::TOTAL); i++) {
		SDL_DestroyTexture(pressTextures[i]);
		pressTextures[i] = nullptr;
	}

	/* Destroy window */
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	/* Quit the SDL library */
	SDL_Quit();
}