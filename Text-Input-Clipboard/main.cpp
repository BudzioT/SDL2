#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "LTexture.h"

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
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

/* Global font */
TTF_Font* font = nullptr;

/* Text textures */
LTexture promptText;
LTexture writeText;


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

	/* Text color */
	SDL_Color textColor = { 0, 0, 0, 0xFF };

	/* Current input text */
	std::string inputText = "Some Text";
	if (!writeText.loadFromRenderedText(inputText.c_str(), textColor))
		printf("Couldn't load input text!\n");

	/* Enable text input */
	SDL_StartTextInput();

	while (!quit) {
		/* Rerender flag */
		bool renderText = false;

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			/* Special key input */
			else if (e.type == SDL_KEYDOWN) {
				/* Handle backspace */
				if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.size() > 0) {
					/* Lop off character */
					inputText.pop_back();
					renderText = true;
				}
				/* Handle copy */
				else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
					SDL_SetClipboardText(inputText.c_str());
				/* Handle paste */
				else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
					/* Copy text from temp buffer */
					char* tempText = SDL_GetClipboardText();
					inputText = tempText;
					SDL_free(tempText);

					renderText = true;
				}
			}

			/* Special key input event */
			else if (e.type == SDL_TEXTINPUT) {
				/* Not copy or pasting */
				if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' ||
					e.text.text[0] == 'v' || e.text.text[0] == 'V'))) {
					/* Append character */
					inputText += e.text.text;
					renderText = true;
				}
			}
		}

		/* Render text if needed */
		if (renderText) {
			/* Text is not empty */
			if (inputText != "") {
				/* Load text */
				writeText.loadFromRenderedText(inputText.c_str(), textColor);
			}
			/* Text is empty */
			else {
				/* Load space texture */
				writeText.loadFromRenderedText(" ", textColor);
			}
		}


		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* Render text */
		promptText.render((SCREEN_WIDTH - promptText.width()) / 2 , 0);
		writeText.render((SCREEN_WIDTH - writeText.width()) / 2, promptText.height());

		/* Update screen */
		SDL_RenderPresent(renderer);
	}

	/* Disable text input */
	SDL_StopTextInput();
	
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

	if (TTF_Init() == -1) {
		printf("Couldn't initalize SDL_TTF!\n");
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

	font = TTF_OpenFont("Fonts/lazy.ttf", 28);
	if (font == nullptr) {
		printf("Couldn't open the lazy font!\n");
		success = false;
	}
	else {
		/* Render the text */
		SDL_Color textColor = { 0, 0, 0, 255 };
		if (!promptText.loadFromRenderedText("Enter Text: ", textColor)) {
			printf("Couldn't render prompt text!\n");
			success = false;
		}
	}

	return success;
}

void close()
{
	/* Free textures */
	promptText.free();
	writeText.free();

	/* Close the font */
	TTF_CloseFont(font);
	font = nullptr;

	/* Destroy window */
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	/* Quit the SDL library */
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
} 