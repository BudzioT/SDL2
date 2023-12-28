#include "Dot.h"

Dot::Dot(int x, int y) : m_velX(0), m_velY(0), m_posX(0), m_posY(0)
{
	/* Load texture */
	if (!m_texture.loadFromFile("Images/dot.bmp"))
		printf("Couldn't load dot texture!\n");
}

Dot::~Dot()
{
	/* Delete texture */
	m_texture.free();
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

void Dot::move(float timeStep)
{
	/* Move the dot left or right */
	m_posX += m_velX * timeStep;

	/* If the dot is too far to the left or right */
	if (m_posX < 0)
		m_posX = 0;
	else if (m_posX > SCREEN_WIDTH - DOT_WIDTH)
		m_posX = SCREEN_WIDTH - DOT_WIDTH;

	/* Move the dot up or down */
	m_posY += m_velY * timeStep;

	/* If the dot is too far up or down */
	if (m_posY < 0)
		m_posY = 0;
	else if (m_posY > SCREEN_HEIGHT - DOT_HEIGHT)
		m_posY = SCREEN_HEIGHT - DOT_HEIGHT;
}

void Dot::render()
{
	/* Show the dot */
	m_texture.render(static_cast<int>(m_posX), static_cast<int>(m_posY));
}