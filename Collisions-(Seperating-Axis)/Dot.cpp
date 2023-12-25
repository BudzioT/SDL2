#include "Dot.h"

Dot::Dot() : m_posX(0), m_posY(0), m_velX(0), m_velY(0)
{
	/* Set collision box dimension */
	m_collider.w = DOT_WIDTH;
	m_collider.h = DOT_HEIGHT;

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

void Dot::move(SDL_Rect& wall)
{
	/* Move the dot left or right */
	m_posX += m_velX;
	m_collider.x = m_posX;

	/* If the dot is too far to the left or right */
	if ((m_posX < 0) || (m_posX + DOT_WIDTH > SCREEN_WIDTH) || checkCollision(m_collider, wall)) {
		/* Move back */
		m_posX -= m_velX;
		m_collider.x = m_posX;
	}

	/* Move the dot up or down */
	m_posY += m_velY;
	m_collider.y = m_posY;

	/* If the dot is too far up or down */
	if ((m_posY < 0) || (m_posY + DOT_HEIGHT > SCREEN_HEIGHT) || checkCollision(m_collider, wall)) {
		/* Move back */
		m_posY -= m_velY;
		m_collider.y = m_posY;
	}
}

void Dot::render()
{
	/* Show the dot */
	texture.render(m_posX, m_posY);
}


bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	/* Calculate the sides of rect A */
	int leftA = a.x,
		rightA = a.x + a.w,
		topA = a.y,
		bottomA = a.y + a.h;
	/* Calculate the sides of rect B */
	int leftB = b.x,
		rightB = b.x + b.w,
		topB = b.y,
		bottomB = b.y + b.h;

	/* If any of the sides from A are outside of B */
	if ((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB))
		return false;

	/* If none of the sides from A are outside of B */
	return true;
}