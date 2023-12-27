#include "Tile.h"

Tile::Tile(int x, int y, Tiles type) : m_type(type)
{
	/* Get the offsets */
	m_box.x = x;
	m_box.y = y;

	/* Set the collision box */
	m_box.w = TILE_WIDTH;
	m_box.h = TILE_HEIGHT;
}

void Tile::render(SDL_Rect& camera)
{
	/* If tile is on screen */
	if (checkCollision(camera, m_box)) {
		/* Show the tile */
		tileTexture.render(m_box.x - camera.x, m_box.y - camera.y, &tileClips[static_cast<int>(m_type)]);
	}
}

Tiles Tile::getType()
{
	return m_type;
}

SDL_Rect Tile::getBox()
{
	return m_box;
}


bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	/* Calculate the sides of A */
	int leftA = a.x,
		rightA = a.x + a.w,
		topA = a.y,
		bottomA = a.y + a.h;
	/* Calculate the sides of B */
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

bool touchesWall(SDL_Rect box, Tile* tiles[])
{
	for (int i = 0; i < TOTAL_TILES; ++i) {
		/* If tile is a wall type */
		if ((tiles[i]->getType() >= Tiles::CENTER) && (tiles[i]->getType() <= Tiles::TOPLEFT)) {
			/* If the collision box touches the wall tile */
			if (checkCollision(box, tiles[i]->getBox()))
				return true;
		}
	}

	/* No wall tiles were touched */
	return false;
}