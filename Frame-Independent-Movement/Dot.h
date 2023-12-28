#pragma once
#include "SDL2/SDL.h"

#include "LTexture.h"

#include <vector>


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
	static const int DOT_VEL = 320;

public:
	Dot(int x = 0, int y = 0);
	~Dot();

	/* Adjust velocity based of key presses */
	void handleEvent(SDL_Event& event);

	/* Move and check collisions with tiles */
	void move(float timeStep);

	/* Show the dot on screen */
	void render();

private:
	/* Velocity */
	float m_velX, m_velY;
	/* Position */
	float m_posX, m_posY;

	/* Texture */
	LTexture m_texture;
};