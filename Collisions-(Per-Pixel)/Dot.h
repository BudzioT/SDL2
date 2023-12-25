#pragma once
#include "SDL2/SDL.h"

#include "LTexture.h"

#include <vector>


/* Screen dimensions */
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

/* Box collision detector */
bool checkCollision(const std::vector<SDL_Rect>& a, const std::vector<SDL_Rect>& b);


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
	void move(const std::vector<SDL_Rect>& otherColliders);

	/* Show the dot on screen */
	void render();

	/* Get dot's collision boxes */
	std::vector<SDL_Rect>& getColliders();

private:
	/* Move the collision boxes relative to the dot's offset */
	void shiftColliders();

private:
	/* X and Y offsets */
	int m_posX, m_posY;
	/* Velocity */
	int m_velX, m_velY;
	/* Texture */
	LTexture texture;
	/* Dot's collision box */
	std::vector<SDL_Rect> m_colliders;
};

