#include "LTexture.h"

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"


LTexture::LTexture() : m_width(0), m_height(0)
{
	/* Initialize */
	m_texture = nullptr;
}

LTexture::~LTexture()
{
	/* Deallocate */
	free();
}

bool LTexture::loadFromFile(const std::string& path)
{
	/* Destroy preexisting texture */
	free();

	/* Load image */
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image_Error: %s\n", path.c_str(), IMG_GetError());
		return false;
	}

	/* Color key image */
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

	/* Create texture from surface pixels */
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (newTexture == NULL)
		printf("Unable to create texture from %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
	else {
		/* Get image dimensions */
		m_width = loadedSurface->w;
		m_height = loadedSurface->h;
	}

	/* Clean up */
	SDL_FreeSurface(loadedSurface);

	m_texture = newTexture;
	/* Return success */
	return m_texture != NULL;
}

void LTexture::free()
{
	/* Free texture if it exists */
	if (m_texture != NULL) {
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
		m_width = 0;
		m_height = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	/* Modulate texture */
	SDL_SetTextureColorMod(m_texture, red, green, blue);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center,
	SDL_RendererFlip flip)
{
	/* Set rendering space and render to screen */
	SDL_Rect renderQuad = { x, y, m_width, m_height };

	/* Set clip rendering dimensions */
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	/* Render to screen */
	SDL_RenderCopyEx(renderer, m_texture, clip, &renderQuad, angle, center, flip);
}

int LTexture::width() const
{
	return m_width;
}

int LTexture::height() const
{
	return m_height;
}

void LTexture::setBlendMode(SDL_BlendMode mode)
{
	/* Set blending function */
	SDL_SetTextureBlendMode(m_texture, mode);
}

void LTexture::setAlpha(Uint8 alpha)
{
	/* Modulate texture alpha */
	SDL_SetTextureAlphaMod(m_texture, alpha);
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	/* Clean up old texture */
	free();

	/* Render text surface */
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurface == NULL) {
		printf("Unable to render text surface! TTF_Error: %s\n", TTF_GetError());
		return false;
	}

	/* Create texture from surface pixels */
	m_texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (m_texture == NULL)
		printf("Unable to create texture from renderer text! SDL_Error: %s\n", SDL_GetError());
	else {
		/* Get image dimensions */
		m_width = textSurface->w;
		m_height = textSurface->h;
	}

	/* Clean up */
	SDL_FreeSurface(textSurface);

	/* Return success */
	return m_texture != NULL;
}