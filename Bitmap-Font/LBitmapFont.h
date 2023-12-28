#pragma once
#include "SDL2/SDL.h"
#include "LTexture.h"

#include <string>

/* Bitmap Font class */
class LBitmapFont
{
public:
	LBitmapFont();

	/* Generate the font */
	bool buildFont(const std::string& path);

	/* Deallocate text */
	void free();

	/* Show the text */
	void renderText(int x, int y, const std::string& text);

private:
	/* The font texture */
	LTexture m_fontTexture;

	/* The individual characters in surface */
	SDL_Rect m_chars[256];

	/* Spacing variables */
	int m_newLine, m_space;
};

