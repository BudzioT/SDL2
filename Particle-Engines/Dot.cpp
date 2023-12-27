#include "Dot.h"

Dot::Dot(int x, int y) : m_posX(x), m_posY(y), m_velX(0), m_velY(0)
{
	/* Load texture */
	if (!m_texture.loadFromFile("Images/dot.bmp"))
		printf("Couldn't load dot texture!\n");

	/* Initialize particles */
	for (int i = 0; i < TOTAL_PARTICLES; i++)
		particles[i] = new Particle(m_posX, m_posY);
}

Dot::~Dot()
{
	/* Delete texture */
	m_texture.free();

	/* Delete particles */
	for (int i = 0; i < TOTAL_PARTICLES; i++)
		delete particles[i];
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

void Dot::move()
{
	/* Move the dot left or right */
	m_posX += m_velX;

	/* If the dot is too far to the left or right */
	if ((m_posX < 0) || (m_posX + DOT_WIDTH > SCREEN_WIDTH)) {
		/* Move back */
		m_posX -= m_velX;
	}

	/* Move the dot up or down */
	m_posY += m_velY;

	/* If the dot is too far up or down */
	if ((m_posY < 0) || (m_posY + DOT_HEIGHT > SCREEN_HEIGHT)) {
		/* Move back */
		m_posY -= m_velY;
	}
}

void Dot::render(int camX, int camY)
{
	/* Show the dot */
	m_texture.render(m_posX - camX, m_posY - camY);

	/* Show particles on top of the dot */
	renderParticles();
}

void Dot::renderParticles()
{
	/* Go through particles */
	for (int i = 0; i < TOTAL_PARTICLES; ++i) {
		/* Delete and replace dead particles */
		if (particles[i]->isDead()) {
			delete particles[i];
			particles[i] = new Particle(m_posX, m_posY);
		}
	}

	/* Show particle */
	for (int i = 0; i < TOTAL_PARTICLES; ++i)
		particles[i]->render();
}