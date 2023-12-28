#pragma once
#include "SDL2/SDL.h"

#include <string>

/* Global window and renderer */
extern SDL_Renderer* renderer;
extern SDL_Window* window;

#if defined(SDL_TTF_MAJOR_VERSION)
/* Global font */
extern TTF_Font* font;
#endif

/* Texture wrapper class */
class LTexture
{
public:
	LTexture();
	~LTexture();

	/* Load texture from given path */
	bool loadFromFile(const std::string& path);

	/* Load image into pixel buffer */
	bool loadPixelsFromFile(const std::string& path);

	/* Create image from preloaded pixels */
	bool loadFromPixels();

#if defined(SDL_TTF_MAJOR_VERSION)
	/* Create image from font string */
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

	/* Deallocate memory */
	void free();

	/* Set color modulation */
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	/* Set blending */
	void setBlendMode(SDL_BlendMode blending);

	/* Set alpha modulation */
	void setAlpha(Uint8 alpha);

	/* Render texture at given point */
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL,
		SDL_RendererFlip flip = SDL_FLIP_NONE);

	/* Get width */
	int width() const;
	/* Get height */
	int height() const;

	/* Pixel accesors */
	Uint32* getPixels32();
	Uint32 getPitch32() const;

private:
	/* The actual hardware texture */
	SDL_Texture* m_texture;
	
	/* Surface pixels */
	SDL_Surface* m_surfacePixels;

	/* Image dimensions */
	int m_width;
	int m_height;
};