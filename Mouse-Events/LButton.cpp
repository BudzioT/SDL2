#include "LButton.h"

LButton::LButton() : m_currentSprite(LButtonSprite::MOUSE_OUT)
{
	m_position.x = 0;
	m_position.y = 0;
}

void LButton::setPosition(int x, int y)
{
	m_position.x = x;
	m_position.y = y;
}

void LButton::handleEvent(SDL_Event* e)
{
	/* If mouse event happened */
	if ((e->type == SDL_MOUSEMOTION) || (e->type == SDL_MOUSEBUTTONDOWN) || (e->type == SDL_MOUSEBUTTONDOWN)) {
		/* Get mouse position */
		int x, y;
		SDL_GetMouseState(&x, &y);

		/* Check if mouse is in button */
		bool inside = true;
		if ((x < m_position.x) || (x > m_position.x + BUTTON_WIDTH) ||
			(y < m_position.y) || (y > m_position.y + BUTTON_HEIGHT)) {
			inside = false;
		}

		/* Mouse is outside */
		if (!inside)
			m_currentSprite = LButtonSprite::MOUSE_OUT;
		/* Mouse is inside button */
		else {
			switch (e->type) {
			case SDL_MOUSEMOTION:
				m_currentSprite = LButtonSprite::MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				m_currentSprite = LButtonSprite::MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				m_currentSprite = LButtonSprite::MOUSE_UP;
				break;
			}
		}
	}
}

void LButton::render(LTexture& texture, SDL_Rect* clips)
{
	texture.render(m_position.x, m_position.y, &clips[static_cast<int>(m_currentSprite)]);
}