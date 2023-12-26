#include "Dot.h"

Dot::Dot(int x, int y) : m_posX(x), m_posY(y), m_velX(0), m_velY(0)
{
	/* Set the collision circle size */
	m_collider.r = DOT_WIDTH / 2;
	
	/* Move collider relative to the circle */
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

void Dot::move(const SDL_Rect& square, const Circle& circle)
{
	/* Move the dot left or right */
	m_posX += m_velX;
	shiftColliders();

	/* If the dot is too far to the left or right */
	if ((m_posX < 0) || (m_posX + DOT_WIDTH > SCREEN_WIDTH) || 
		checkCollision(m_collider, square) || checkCollision(m_collider, circle)) {
		/* Move back */
		m_posX -= m_velX;
		shiftColliders();
	}

	/* Move the dot up or down */
	m_posY += m_velY;
	shiftColliders();

	/* If the dot is too far up or down */
	if ((m_posY < 0) || (m_posY + DOT_HEIGHT > SCREEN_HEIGHT) || 
		checkCollision(m_collider, square) || checkCollision(m_collider, circle)) {
		/* Move back */
		m_posY -= m_velY;
		shiftColliders();
	}
}

void Dot::render()
{
	/* Show the dot */
	texture.render(m_posX - m_collider.r, m_posY - m_collider.r);
}

void Dot::shiftColliders()
{
	m_collider.x = m_posX;
	m_collider.y = m_posY;
}

Circle& Dot::getColliders()
{
	return m_collider;
}


bool checkCollision(const Circle& a, const Circle& b)
{
	/* Calculate total radius squared */
	int totalRadiusSquared = a.r + b.r;
	totalRadiusSquared *= totalRadiusSquared;

	/* If the distance between the centers of circles is less than the sum of their radii */
	if (distanceSquared(a.x, a.y, b.x, b.y) < totalRadiusSquared) {
		/* The circles have collided */
		return true;
	}

	/* No collisions */
	return false;
}

bool checkCollision(const Circle& a, const SDL_Rect& b)
{
	/* Closes point on collision box */
	int cX, cY;

	/* Find closest x offset */
	if (a.x < b.x)
		cX = b.x;
	else if (a.x > b.x + b.w)
		cX = b.x + b.w;
	else
		cX = a.x;

	/* Find closest y offset */
	if (a.y < b.y)
		cY = b.y;
	else if (a.y > b.y + b.h)
		cY = b.y + b.h;
	else
		cY = a.y;

	/* If the closest point is inside the circle */
	if (distanceSquared(a.x, a.y, cX, cY) < (a.r * a.r)) {
		/* This circle and box have collided */
		return true;
	}
	
	/* No collision */
	return false;
}

double distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return (deltaX * deltaX) + (deltaY * deltaY);
}