#pragma once
#include "SDL2/SDL.h"
#include "LTexture.h"

/* Button constants */
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;
enum class LButtonSprite {
	MOUSE_OUT,
	MOUSE_OVER_MOTION,
	MOUSE_DOWN,
	MOUSE_UP,
	TOTAL
};


/* The mouse button */
class LButton
{
public:
	LButton();

	/* Set top left position */
	void setPosition(int x, int y);

	/* Handle mouse event */
	void handleEvent(SDL_Event* e);

	/* Show button sprite */
	void render(LTexture& texture, SDL_Rect* clips = nullptr);

private:
	/* Top left position */
	SDL_Point m_position;
	/* Currently used global sprite */
	LButtonSprite m_currentSprite;
};

