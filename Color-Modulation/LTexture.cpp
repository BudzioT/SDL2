#include "LTexture.h"

#include "SDL2/SDL_image.h"


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

void LTexture::render(int x, int y, SDL_Rect* clip)
{
	/* Set rendering space and render to screen */
	SDL_Rect renderQuad = { x, y, m_width, m_height };

	/* Set clip rendering dimensions */
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	/* Render to screen */
	SDL_RenderCopy(renderer, m_texture, clip, &renderQuad);
}

int LTexture::width() const
{
	return m_width;
}

int LTexture::height() const
{
	return m_height;
}