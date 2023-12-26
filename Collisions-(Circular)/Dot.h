#pragma once
#include "SDL2/SDL.h"

#include "LTexture.h"

#include <vector>


/* Screen dimensions */
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

/* Circle struct */
struct Circle {
	int x, y;
	int r;
};

/* Circle-Circle collision detector */
bool checkCollision(const Circle& a, const Circle& b);
/* Circle-Box collision detector */
bool checkCollision(const Circle& a, const SDL_Rect& b);
/* Calculate distance squared between two points */
double distanceSquared(int x1, int y1, int x2, int y2);


/* Dot that will move around on the screen */
class Dot
{
public:
	/* Dimensions */
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;
	/* Maximum axis velocity*/
	static const int DOT_VEL = 1;

public:
	Dot(int x, int y);
	~Dot();

	/* Adjust velocity based of key presses */
	void handleEvent(SDL_Event& event);

	/* Move */
	void move(const SDL_Rect& square, const Circle& circle);

	/* Show the dot on screen */
	void render();

	/* Get dot's collision boxes */
	Circle& getColliders();

private:
	/* Move the collision circle relative to the dot's offset */
	void shiftColliders();

private:
	/* X and Y offsets */
	int m_posX, m_posY;
	/* Velocity */
	int m_velX, m_velY;
	/* Texture */
	LTexture texture;
	/* Dot's collision box */
	Circle m_collider;
};