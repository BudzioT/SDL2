#include "LBitmapFont.h"

LBitmapFont::LBitmapFont() : m_newLine(0), m_space(0)
{
}

bool LBitmapFont::buildFont(const std::string& path)
{
	/* Free preexisting texture */
	free();

	/* Load bitmap image */
	bool success = true;
	if (!m_fontTexture.loadPixelsFromFile(path)) {
		printf("Couldn't load bitmap font surface!\n");
		success = false;
	}
	else {
		/* Get the background color */
		Uint32 bgColor = m_fontTexture.getPixel32(0, 0);

		/* Set the cell dimensions */
		int cellW = m_fontTexture.width() / 16;
		int cellH = m_fontTexture.height() / 16;

		/* New line variables */
		int top = cellH;
		int baseA = cellH;

		/* The current character set */
		int currentChar = 0;

		/* Go through the cell rows */
		for (int rows = 0; rows < 16; ++rows) {
			/* Go through cell columns */
			for (int cols = 0; cols < 16; ++cols) {
				/* Set character offset */
				m_chars[currentChar].x = cellW * cols;
				m_chars[currentChar].y = cellH * rows;

				/* Set the dimensions of the character */
				m_chars[currentChar].w = cellW;
				m_chars[currentChar].h = cellH;

				/* Find left side */
				/* Go through pixel columns */
				for (int pCol = 0; pCol < cellW; ++pCol) {
					/* Go through pixel rows */
					for (int pRow = 0; pRow < cellH; ++pRow) {
						/* Get pixel offsets */
						int pX = (cellW * cols) + pCol;
						int pY = (cellH * rows) + pRow;

						/* If non-color key pixel is found */
						if (m_fontTexture.getPixel32(pX, pY) != bgColor) {
							/* Set the x offset */
							m_chars[currentChar].x = pX;

							/* Break the loop */
							pCol = cellW;
							pRow = cellH;
						}
					}

					/* Find right side */
					/* Go through pixel columns */
					for (int pColW = cellW - 1; pColW >= 0; --pColW) {
						/* Go through pixel rows */
						for (int pRowW = 0; pRowW < cellH; ++pRowW) {
							/* Get pixel offsets */
							int pX = (cellW * cols) + pColW;
							int pY = (cellH * rows) + pRowW;

							/* If non-color key pixel is found */
							if (m_fontTexture.getPixel32(pX, pY) != bgColor) {
								/* Set the width */
								m_chars[currentChar].w = (pX - m_chars[currentChar].x) + 1;

								/* Break the loop */
								pColW = -1;
								pRowW = cellH;
							}
						}
					}

					/* Find top */
					/* Go through pixel rows */
					for (int pRow = 0; pRow < cellH; ++pRow) {
						/* Go through pixel columns */
						for (int pCol = 0; pCol < cellW; ++pCol) {
							/* Get pixel offsets */
							int pX = (cellW * cols) + pCol;
							int pY = (cellH * rows) + pRow;

							/* If non-color key pixel is found */
							if (m_fontTexture.getPixel32(pX, pY) != bgColor) {
								/* If new top is found */
								if (pRow < top)
									top = pRow;

								/* Break the loop */
								pRow = cellH;
								pCol = cellW;
							}
						}
					}

					/* Find bottom of 'A' */
					if (currentChar == 'A') {
						/* Go through pixel rows */
						for (int pRow = cellH - 1; pRow >= 0; --pRow) {
							/* Go through pixel columns */
							for (int pCol = 0; pCol < cellW; ++pCol) {
								/* Get pixel offsets */
								int pX = (cellW * cols) + pCol;
								int pY = (cellH * rows) + pRow;

								/* If non-color key pixel is found */
								if (m_fontTexture.getPixel32(pX, pY) != bgColor) {
									/* Bottom of 'A' is found */
									baseA = pRow;

									/* Break the loop */
									pCol = cellW;
									pRow = -1;
								}
							}
						}
					}
				}

				/* Go to the next character */
				++currentChar;
			}
		}

		/* Calculate space */
		m_space = cellW / 2;

		/* Calculate new line */
		m_newLine = baseA - top;

		/* Lop off excess top pixels */
		for (int i = 0; i < 256; i++) {
			m_chars[i].y += top;
			m_chars[i].h -= top;
		}

		/* Create final texture */
		if (!m_fontTexture.loadFromPixels()) {
			printf("Couldn't create font texture!\n");
			success = false;
		}
	}

	return success;
}

void LBitmapFont::renderText(int x, int y, const std::string& text)
{
	/* If the font has been built */
	if (m_fontTexture.width() > 0) {
		/* Temp offsets */
		int curX = x, curY = y;

		/* Go through the text */
		for (int i = 0; i < text.length(); ++i) {
			/* If the current character is a space */
			if (text[i] == ' ') {
				/* Move over */
				curX += m_space;
			}
			/* If the current character is a newline*/
			else if (text[i] == '\n') {
				/* Move down */
				curY += m_newLine;

				/* Move back */
				curX = x;
			}

			else {
				/* Get the ASCII value of the character */
				int ascii = (unsigned char)text[i];

				/* Show the character */
				m_fontTexture.render(curX, curY, &m_chars[ascii]);

				/* Move over the width of the character with one pixel of padding */
				curX += m_chars[ascii].w + 1;
			}
		}
	}
}

void LBitmapFont::free()
{
	m_fontTexture.free();
}