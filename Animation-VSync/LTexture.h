#pragma once
#include "SDL2/SDL.h"

#include <string>

/* Global renderer */
extern SDL_Renderer* renderer;

/* Texture wrapper class */
class LTexture
{
public:
	LTexture();
	~LTexture();

	/* Load texture from given path */
	bool loadFromFile(const std::string& path);

	/* Deallocate memory */
	void free();

	/* Set color modulation */
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	/* Set blending */
	void setBlendMode(SDL_BlendMode blending);

	/* Set alpha modulation */
	void setAlpha(Uint8 alpha);

	/* Render texture at given point */
	void render(int x, int y, SDL_Rect* clip = NULL);

	/* Get width */
	int width() const;
	/* Get height */
	int height() const;

private:
	/* The actual hardware texture */
	SDL_Texture* m_texture;
	/* Image dimensions */
	int m_width;
	int m_height;
};

