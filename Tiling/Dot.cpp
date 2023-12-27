#include "Dot.h"

Dot::Dot(int x, int y) : m_velX(0), m_velY(0)
{
	m_box.x = 0;
	m_box.y = 0;

	m_box.w = DOT_WIDTH;
	m_box.h = DOT_HEIGHT;

	/* Load texture */
	if (!m_texture.loadFromFile("Images/dot.bmp"))
		printf("Couldn't load dot texture!\n");

	/* Initialize particles */
	for (int i = 0; i < TOTAL_PARTICLES; i++)
		particles[i] = new Particle(m_box.x, m_box.y);
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

void Dot::move(Tile* tiles[])
{
	/* Move the dot left or right */
	m_box.x += m_velX;

	/* If the dot is too far to the left or right */
	if ((m_box.x < 0) || (m_box.x + DOT_WIDTH > LEVEL_WIDTH) || touchesWall(m_box, tiles)) {
		/* Move back */
		m_box.x -= m_velX;
	}

	/* Move the dot up or down */
	m_box.y += m_velY;

	/* If the dot is too far up or down */
	if ((m_box.y < 0) || (m_box.y + DOT_HEIGHT > LEVEL_HEIGHT) || touchesWall(m_box, tiles)) {
		/* Move back */
		m_box.y -= m_velY;
	}
}

void Dot::setCamera(SDL_Rect& camera)
{
	/* Center camera over the dot */
	camera.x = (m_box.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (m_box.y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	/* Keep the camera in bounds */
	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > LEVEL_WIDTH - camera.w)
		camera.x = LEVEL_WIDTH - camera.w;
	if (camera.y > LEVEL_HEIGHT - camera.h)
		camera.y = LEVEL_HEIGHT - camera.h;
}

void Dot::render(const SDL_Rect& camera)
{
	/* Show the dot */
	m_texture.render(m_box.x - camera.x, m_box.y - camera.y);

	/* Show particles on top of the dot */
	renderParticles(camera);
}

void Dot::renderParticles(const SDL_Rect& camera)
{
	/* Go through particles */
	for (int i = 0; i < TOTAL_PARTICLES; ++i) {
		/* Delete and replace dead particles */
		if (particles[i]->isDead()) {
			delete particles[i];
			particles[i] = new Particle(m_box.x - camera.x, m_box.y - camera.y);
		}
	}

	/* Show particle */
	for (int i = 0; i < TOTAL_PARTICLES; ++i)
		particles[i]->render();
}