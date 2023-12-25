#include "Dot.h"

Dot::Dot(int x, int y) : m_posX(x), m_posY(y), m_velX(0), m_velY(0)
{
	/* Create the necessary SDL_Rects */
	m_colliders.resize(11);

	/* Initialize the collision boxes' width and height */
	m_colliders[0].w = 6;
	m_colliders[0].h = 1;

	m_colliders[1].w = 10;
	m_colliders[1].h = 1;

	m_colliders[2].w = 14;
	m_colliders[2].h = 1;

	m_colliders[3].w = 16;
	m_colliders[3].h = 2;

	m_colliders[4].w = 18;
	m_colliders[4].h = 2;

	m_colliders[5].w = 20;
	m_colliders[5].h = 6;

	m_colliders[6].w = 18;
	m_colliders[6].h = 2;

	m_colliders[7].w = 16;
	m_colliders[7].h = 2;

	m_colliders[8].w = 14;
	m_colliders[8].h = 1;

	m_colliders[9].w = 10;
	m_colliders[9].h = 1;

	m_colliders[10].w = 6;
	m_colliders[10].h = 1;
	
	/* Initialize colliders relative to position */
	shiftColliders();

	/* Load texture */
	if (!texture.loadFromFile("Images/dot.bmp"))
		printf("Couldn't load dot texture!\n");
}

Dot::~Dot()
{
	texture.free();
}

void Dot::handleEvent(SDL_Event& event)
{
	/* If a key was pressed */
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		/* Adjust the velocity */
		switch (event.key.keysym.sym) {
		case SDLK_UP:
			m_velY -= DOT_VEL;
			break;
		case SDLK_DOWN:
			m_velY += DOT_VEL;
			break;
		case SDLK_LEFT:
			m_velX -= DOT_VEL;
			break;
		case SDLK_RIGHT:
			m_velX += DOT_VEL;
			break;
		}
	}

	else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
		/* Adjust the velocity */
		switch (event.key.keysym.sym) {
		case SDLK_UP:
			m_velY += DOT_VEL;
			break;
		case SDLK_DOWN:
			m_velY -= DOT_VEL;
			break;
		case SDLK_LEFT:
			m_velX += DOT_VEL;
			break;
		case SDLK_RIGHT:
			m_velX -= DOT_VEL;
			break;
		}
	}
}

void Dot::move(const std::vector<SDL_Rect>& otherColliders)
{
	/* Move the dot left or right */
	m_posX += m_velX;
	shiftColliders();

	/* If the dot is too far to the left or right */
	if ((m_posX < 0) || (m_posX + DOT_WIDTH > SCREEN_WIDTH) || checkCollision(m_colliders, otherColliders)) {
		/* Move back */
		m_posX -= m_velX;
		shiftColliders();
	}

	/* Move the dot up or down */
	m_posY += m_velY;
	shiftColliders();

	/* If the dot is too far up or down */
	if ((m_posY < 0) || (m_posY + DOT_HEIGHT > SCREEN_HEIGHT) || checkCollision(m_colliders, otherColliders)) {
		/* Move back */
		m_posY -= m_velY;
		shiftColliders();
	}
}

void Dot::render()
{
	/* Show the dot */
	texture.render(m_posX, m_posY);
}

void Dot::shiftColliders()
{
	/* The row offset */
	int r = 0;

	/* Go through the dot's collision boxes */
	for (int set = 0; set < m_colliders.size(); set++) {
		/* Center the collision box */
		m_colliders[set].x = m_posX + (DOT_WIDTH - m_colliders[set].w) / 2;

		/* Set the collision box at its row offset */
		m_colliders[set].y = m_posY + r;

		/* Move the row offset down the height of collision box */
		r += m_colliders[set].h;
	}
}

std::vector<SDL_Rect>& Dot::getColliders()
{
	return m_colliders;
}


bool checkCollision(const std::vector<SDL_Rect>& a, const std::vector<SDL_Rect>& b)
{
	/* Sides of rectangles */
	int leftA, leftB,
		rightA, rightB,
		topA, topB,
		bottomA, bottomB;

	/* Go through the A boxes */
	for (int Abox = 0; Abox < a.size(); Abox++) {
		/* Calculate the sides of rect A */
		const SDL_Rect* rectA = &a[Abox];
		leftA = rectA->x;
		rightA = rectA->x + rectA->w;
		topA = rectA->y;
		bottomA = rectA->y + rectA->h;

		/* Go through the B boxes */
		for (int BBox = 0; BBox < b.size(); BBox++) {
			const SDL_Rect* rectB = &b[BBox];
			leftB = rectB->x;
			rightB = rectB->x + rectB->w;
			topB = rectB->y;
			bottomB = rectB->y + rectB->h;

			/* If no sides from A are outside of B */
			if (((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB)) == false) {
				/* A collision is detected */
				return true;
			}
		}
	}

	/* If neither set of collision boxes touched */
	return false;
}