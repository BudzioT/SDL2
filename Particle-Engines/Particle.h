#pragma once
#include "SDL2/SDL.h"
#include "LTexture.h"

const int TOTAL_PARTICLES = 20;

extern LTexture redTexture;
extern LTexture greenTexture;
extern LTexture blueTexture;
extern LTexture shimmerTexture;

class Particle
{
public:
	/* Initialize position and animation */
	Particle(int x, int y);

	/* Show the particle */
	void render();

	/* Check if particle is dead */
	bool isDead();

private:
	/* Offsets */
	int m_posX, m_posY;

	/* Current frame of animation */
	int m_frame;

	/* Type of particle */
	LTexture* m_texture;
};

