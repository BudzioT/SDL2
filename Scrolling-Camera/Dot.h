#pragma once
#include "SDL2/SDL.h"

#include "LTexture.h"

#include <vector>


/* Screen dimensions */
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

/* Level dimensions */
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;


/* Dot that will move around on the screen */
class Dot
{
public:
	/* Dimensions */
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;
	/* Maximum axis velocity*/
	static const int DOT_VEL = 5;

public:
	Dot(int x = 0, int y = 0);
	~Dot();

	/* Adjust velocity based of key presses */
	void handleEvent(SDL_Event& event);

	/* Move */
	void move();

	/* Show the dot on screen */
	void render(int camX, int camY);

	/* Position accessors */
	int getPosX() const;
	int getPosY() const;

private:
	/* X and Y offsets */
	int m_posX, m_posY;
	/* Velocity */
	int m_velX, m_velY;
	/* Texture */
	LTexture m_texture;
};