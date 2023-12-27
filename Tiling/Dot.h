#pragma once
#include "SDL2/SDL.h"

#include "LTexture.h"
#include "Particle.h"
#include "Tile.h"

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

	/* Move and check collisions with tiles */
	void move(Tile *tiles[]);

	/* Center camera over the dot */
	void setCamera(SDL_Rect& camera);

	/* Show the dot on screen */
	void render(const SDL_Rect& camera);

private:
	/* Shows the particles */
	void renderParticles(const SDL_Rect& camera);

private:
	/* Velocity */
	int m_velX, m_velY;

	/* Collision box of dot */
	SDL_Rect m_box;

	/* Texture */
	LTexture m_texture;

	/* Particles */
	Particle* particles[TOTAL_PARTICLES];
};