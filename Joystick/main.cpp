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
LTexture arrowTexture;

/* Analog joystick dead zone */
const int JOYSTICK_DEAD_ZONE = 8000;

/* Game controller 1 handler */
SDL_Joystick* gameController = nullptr;


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

			else if (e.type == SDL_JOYAXISMOTION) {
				/* Motion on controller 0 */
				if (e.jaxis.which == 0) {
					/* X axis motion */
					if (e.jaxis.axis == 0) {
						/* Left of dead zone */
						if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
							xDir = -1;
						/* Right of dead zone */
						else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
							xDir = 1;
						else
							xDir = 0;
					}

					/* Y axis motion */
					else if (e.jaxis.axis == 1) {
						/* Below of dead zone */
						if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
							yDir = -1;
						/* Above of dead zone */
						else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
							yDir = 1;
						else
							yDir = 0;
					}
				}
			}
		}

		/* Calculate angle */
		double joystickAngle = atan2(static_cast<double>(yDir), static_cast<double>(xDir)) * (180.0 / M_PI);

		/* Correct angle */
		if (xDir == 0 && yDir == 0)
			joystickAngle = 0;

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);


		/* Render joystick 8 way angle */
		arrowTexture.render((SCREEN_WIDTH - arrowTexture.width()) / 2,
			(SCREEN_HEIGHT - arrowTexture.height()) / 2, nullptr, joystickAngle);
		
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
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
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
		/* Load joystick */
		gameController = SDL_JoystickOpen(0);
		if (gameController == NULL)
			printf("Warning: Unable to open game controller! SDL_Error: %s\n", SDL_GetError());
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

	if (!arrowTexture.loadFromFile("Images/arrow.png")) {
		printf("Couldn't load arrow texture!\n");
		success = false;
	}

	return success;
}

void close()
{
	/* Free loaded textures */
	arrowTexture.free();

#if defined(SDL_TTF_MAJOR_VERSION)
	/* Free global font */
	TTF_CloseFont(font);
	font = nullptr;
#endif

	/* Close game controller */
	SDL_JoystickClose(gameController);
	gameController = nullptr;

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