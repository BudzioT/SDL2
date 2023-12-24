#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#include "LTexture.h"

#include <stdio.h>
#include <string>


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

/* Textures */
LTexture splashTexture;

/* Music that will be played */
Mix_Music* music = nullptr;

/* The sound effect that will be used */
Mix_Chunk* scratch = nullptr;
Mix_Chunk* high = nullptr;
Mix_Chunk* medium = nullptr;
Mix_Chunk* low = nullptr;


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

			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					/* Play high sound effect */
				case SDLK_1:
					Mix_PlayChannel(-1, high, 0);
					break;

					/* Play medium sound effect */
				case SDLK_2:
					Mix_PlayChannel(-1, medium, 0);
					break;

					/* Play low sound effect */
				case SDLK_3:
					Mix_PlayChannel(-1, low, 0);
					break;

					/* Play scratch sound effect */
				case SDLK_4:
					Mix_PlayChannel(-1, scratch, 0);
					break;

				case SDLK_9:
					/* If there isn't music playing */
					if (Mix_PlayingMusic() == 0) {
						/* Play music */
						Mix_PlayMusic(music, -1);
					}
					/* If music is being played */
					else {
						/* If the music is paused */
						if (Mix_PausedMusic() == 1) {
							/* Resume the music */
							Mix_ResumeMusic();
						}
						/* If the music is playing */
						else {
							/* Pause the music */
							Mix_PauseMusic();
						}
					}
					break;

				case SDLK_0:
					/* Stop the music */
					Mix_HaltMusic();
					break;
				}
			}
		}

		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		
		/* Render splash screen texture */
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
	bool success = true;

	/* Initialize video */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL couldn't initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	/* Create window */
	window = SDL_CreateWindow("Key Presses optimized", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window couldn't be created! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	/* Create VSync renderer for window */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

	/* Initialize SDL_mixer */
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer couldn't initialize! SDL_mixer_Error: %s\n", Mix_GetError());
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

	/* Load splash screen texture */
	if (!splashTexture.loadFromFile("Images/splash.png")) {
		printf("Couldn't load splash screen texture!\n");
		success = false;
	}

	/* Load music */
	music = Mix_LoadMUS("Music/beat.wav");
	if (music == nullptr) {
		printf("Failed to load beat music! SDL_mix_Error: %s\n", Mix_GetError());
		success = false;
	}

	/* Load sound effects */
	scratch = Mix_LoadWAV("Sounds/scratch.wav");
	if (scratch == nullptr) {
		printf("Failed to load scratch sound effect! SDL_mix_Error: %s\n", Mix_GetError());
		success = false;
	}

	high = Mix_LoadWAV("Sounds/high.wav");
	if (high == nullptr) {
		printf("Failed to load high sound effect! SDL_mix_Error: %s\n", Mix_GetError());
		success = false;
	}

	medium = Mix_LoadWAV("Sounds/medium.wav");
	if (medium == nullptr) {
		printf("Failed to load medium sound effect! SDL_mix_Error: %s\n", Mix_GetError());
		success = false;
	}

	low = Mix_LoadWAV("Sounds/low.wav");
	if (low == nullptr) {
		printf("Failed to load low sound effect! SDL_mix_Error: %s\n", Mix_GetError());
		success = false;
	}

	return success;
}

void close()
{
	/* Free loaded textures */
	splashTexture.free();

	/* Free the sound effects */
	Mix_FreeChunk(scratch);
	Mix_FreeChunk(high);
	Mix_FreeChunk(medium);
	Mix_FreeChunk(low);
	scratch = high = medium = low = nullptr;

	/* Free the music */
	Mix_FreeMusic(music);
	music = nullptr;
	
	/* Destroy window */
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	/* Quit the SDL library */
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
} 