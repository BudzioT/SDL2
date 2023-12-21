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
	int surface = static_cast<int>(KeyPressSurfaces::DEFAULT);
	currentTexture = pressTextures[surface];
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_UP:
					surface = static_cast<int>(KeyPressSurfaces::UP);
					break;
				case SDLK_DOWN:
					surface = static_cast<int>(KeyPressSurfaces::DOWN);
					break;
				case SDLK_LEFT:
					surface = static_cast<int>(KeyPressSurfaces::LEFT);
					break;
				case SDLK_RIGHT:
					surface = static_cast<int>(KeyPressSurfaces::RIGHT);
					break;
				default:
					surface = static_cast<int>(KeyPressSurfaces::DEFAULT);
					break;
				}

				currentTexture = pressTextures[surface];
			}
		}

		

		/* Clear screen */
		SDL_RenderClear(renderer);
		/* Render texture to screen */
		SDL_RenderCopy(renderer, currentTexture, NULL, NULL);
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

	int action = static_cast<int>(KeyPressSurfaces::DEFAULT);
	pressTextures[action] = IMG_LoadTexture(renderer, "Images/Default.bmp");
	if (pressTextures[action] == NULL) {
		printf("Failed to load the default image!\n");
		success = false;
	}

	action = static_cast<int>(KeyPressSurfaces::UP);
	pressTextures[action] = IMG_LoadTexture(renderer, "Images/Up.png");
	if (pressTextures[action] == NULL) {
		printf("Failed to load the up image!\n");
		success = false;
	}

	action = static_cast<int>(KeyPressSurfaces::DOWN);
	pressTextures[action] = loadTexture("Images/Down.bmp");
	if (pressTextures[action] == NULL) {
		printf("Failed to load the down image!\n");
		success = false;
	}

	action = static_cast<int>(KeyPressSurfaces::LEFT);
	pressTextures[action] = loadTexture("Images/Left.bmp");
	if (pressTextures[action] == NULL) {
		printf("Failed to load the left image!\n");
		success = false;
	}

	action = static_cast<int>(KeyPressSurfaces::RIGHT);
	pressTextures[action] = loadTexture("Images/Right.bmp");
	if (pressTextures[action] == NULL) {
		printf("Failed to load the right image!\n");
		success = false;
	}

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