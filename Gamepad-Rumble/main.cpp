#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "LTexture.h"

#include <stdio.h>
#include <string>
#include <cmath>


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

#if defined(SDL_TTF_MAJOR_VERSION)
/* Global font */
TTF_Font* font = NULL;
#endif

/* Textures */
LTexture splashTexture;

/* Game controller handler with force feedback */
SDL_GameController* gameController = nullptr;

/* [OLD WAY] Joystick handler with haptic */
SDL_Joystick* joystick = nullptr;
SDL_Haptic* joyHaptic = nullptr;


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

	/* Normalized directions */
	int xDir = 0;
	int yDir = 0;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			/* Joystick button press */
			else if (e.type == SDL_JOYBUTTONDOWN) {
				/* Use game controller */
				if (gameController != nullptr) {
					/* Play rumble at 75% strength for 500 miliseconds */
					if (SDL_GameControllerRumble(gameController, 0xFFFF * 0.75, 0xFFFF * 0.75, 500) != 0) {
						printf("Warning: Unable to play game controller rumble! SDL_Error: %s\n",
							SDL_GetError());
					}
				}
				/* Use haptics */
				else if (joyHaptic != nullptr) {
					/* Play rumble at 75% strength for 500 miliseconds */
					if (SDL_HapticRumblePlay(joyHaptic, 0.75, 500) != 0)
						printf("Warning: Unable to play haptic rumble! SDL_Error: %s\n", SDL_GetError());
				}
			}
		}

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* Render splash texture */
		splashTexture.render(0, 0);
		
		/* Update screen */
		SDL_RenderPresent(renderer);
	}
	
	/* Clean up */
	close();
	return 0;
}


bool init()
{
	/* Initialize video and joystick */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) < 0) {
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

	/* Create VSync renderer for window */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

	/* Set texture filtering to linear */
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		printf("Warning: Linear texture filtering not enabled!\n");
	
	/* Check for joysticks */
	if (SDL_NumJoysticks() < 1)
		printf("Warning: No joysticks connected!\n");
	else {
		/* Check if first joystick is game controller interface compatible */
		if (!SDL_IsGameController(0)) {
			printf("Warning: Joystick is not game controller interface compatible! SDL_Error: %s\n",
				SDL_GetError());

			/* Load joystick if game controller couldn't be loaded */
			joystick = SDL_JoystickOpen(0);
			if (joystick == nullptr)
				printf("Warning: Unable to open joystick! SDL_Error: %s\n", SDL_GetError());
			else {
				/* Get joystick haptic device */
				joyHaptic = SDL_HapticOpenFromJoystick(joystick);
				if (joyHaptic == nullptr)
					printf("Warning: Unable to get joystick haptics! SDL_Error: %s\n", SDL_GetError());
				else {
					/* Initialize rumble */
					if (SDL_HapticRumbleInit(joyHaptic) < 0)
						printf("Warning: Unable to initialize haptic rumble! SDL_Error: %s\n", SDL_GetError());
				}
			}
		}
		else {
			/* Open game controller and check if it supports rumble */
			gameController = SDL_GameControllerOpen(0);
			if (!SDL_GameControllerHasRumble(gameController))
				printf("Warning: Game controller doesn't have rumble! SDL_Error: %s\n", SDL_GetError());
		}
	}

#if defined(SDL_TTF_MAJOR_VERSION)
	/* Initialize SDL_ttf */
	if (TTF_Init() == -1) {
		printf("SDL_TTF coudln't initalize! TTF_Error: %s\n", TTF_GetError());
		return false;
	}
#endif

	return true;
}

bool loadMedia()
{
	bool success = true;

	if (!splashTexture.loadFromFile("Images/rumble.png")) {
		printf("Couldn't load arrow texture!\n");
		success = false;
	}

	return success;
}

void close()
{
	/* Free loaded textures */
	splashTexture.free();

#if defined(SDL_TTF_MAJOR_VERSION)
	/* Free global font */
	TTF_CloseFont(font);
	font = nullptr;
#endif

	/* Close game controller or joystick with haptics */
	if (gameController != nullptr) {
		SDL_GameControllerClose(gameController);
		gameController = nullptr;
	}
	if (joyHaptic != nullptr) {
		SDL_HapticClose(joyHaptic);
		joyHaptic = nullptr;
	}
	if (joystick != nullptr) {
		SDL_JoystickClose(joystick);
		joystick = nullptr;
	}
	
	/* Destroy window */
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	/* Quit the SDL library */
#if defined(SDL_TTF_MAJOR_VERSION)
	TTF_Quit();
#endif
	IMG_Quit();
	SDL_Quit();
} 