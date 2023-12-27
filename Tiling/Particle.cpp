#include "Particle.h"

Particle::Particle(int x, int y)
{
	/* Set offsets */
	m_posX = x - 5 + (rand() % 25);
	m_posY = y - 5 + (rand() % 25);

	/* Initialize animation */
	m_frame = rand() % 5;

	/* Set type */
	switch (rand() % 3) {
	case 0:
		m_texture = &redTexture;
		break;
	case 1:
		m_texture = &greenTexture;
		break;
	case 2:
		m_texture = &blueTexture;
		break;
	}
}

void Particle::render()
{
	/* Show image */
	m_texture->render(m_posX, m_posY);

	/* Show shimmer */
	if (m_frame % 2 == 0)
		shimmerTexture.render(m_posX, m_posY);
	
	/* Animate */
	++m_frame;
}

bool Particle::isDead()
{
	return m_frame > 10;
}