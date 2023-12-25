#pragma once
#include "SDL2/SDL.h"

#include "LTexture.h"

/* Screen dimensions */
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

/* Dot that will move around on the screen */
class Dot
{
public:
	/* Dimensions */
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;
	/* Maximum axis velocity*/
	static const int DOT_VEL = 10;

public:
	Dot();
	~Dot();

	/* Adjust velocity based of key presses */
	void handleEvent(SDL_Event& event);

	/* Move */
	void move();

	/* Show the dot on screen */
	void render();

private:
	/* X and Y offsets */
	int m_posX, m_posY;
	/* Velocity */
	int m_velX, m_velY;
	/* Texture */
	LTexture texture;
};

