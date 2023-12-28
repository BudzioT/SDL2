#include "LTexture.h"

#include "SDL2/SDL_image.h"


LTexture::LTexture() : m_width(0), m_height(0), m_texture(nullptr), m_surfacePixels(nullptr)
{
}

LTexture::~LTexture()
{
	/* Deallocate */
	free();
}

bool LTexture::loadFromFile(const std::string& path)
{
	if (!loadPixelsFromFile(path))
		printf("Couldn't load pixels for %s!\n", path.c_str());
	else {
		/* Load texture from pixels */
		if (!loadFromPixels())
			printf("Couldn't load texture from pixels from %s!\n", path.c_str());
	}

	return m_texture;
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

	/* Free surface if it exists */
	if (m_surfacePixels) {
		SDL_FreeSurface(m_surfacePixels);
		m_surfacePixels = nullptr;
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

#if defined(SDL_TTF_MAJOR_VERSION)
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
#endif

bool LTexture::loadPixelsFromFile(const std::string& path)
{
	/* Free preexisting texture */
	free();

	/* Load image at specified path */
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (!loadedSurface)
		printf("Couldn't load image %s! IMG_Error: %s\n", path.c_str(), IMG_GetError());
	else {
		m_surfacePixels = SDL_ConvertSurfaceFormat(loadedSurface, SDL_GetWindowPixelFormat(window), 0);
		if (!m_surfacePixels)
			printf("Couldn't convert loaded surface to display format! SDL_Error: %s\n", SDL_GetError());
		else {
			/* Get image dimensions */
			m_width = m_surfacePixels->w;
			m_height = m_surfacePixels->h;
		}

		/* Clean up */
		SDL_FreeSurface(loadedSurface);
	}

	return m_surfacePixels;
}

bool LTexture::loadFromPixels()
{
	/* Load only if pixels exists */
	if (!m_surfacePixels) {
		printf("No pixels loaded!\n");
		return false;
	}

	/* Color key image */
	SDL_SetColorKey(m_surfacePixels, SDL_TRUE, SDL_MapRGB(m_surfacePixels->format, 0, 0xFF, 0xFF));

	/* Create texture from surface pixels */
	m_texture = SDL_CreateTextureFromSurface(renderer, m_surfacePixels);
	if (!m_texture)
		printf("Couldn't create texture from loaded pixels! SDL_Error: %s\n", SDL_GetError());
	else {
		/* Get image dimensions */
		m_width = m_surfacePixels->w;
		m_height = m_surfacePixels->h;
	}

	/* Clean up old surface */
	SDL_FreeSurface(m_surfacePixels);
	m_surfacePixels = nullptr;

	return m_texture;
}

Uint32* LTexture::getPixels32()
{
	Uint32* pixels = nullptr;
	if (m_surfacePixels)
		pixels = static_cast<Uint32*>(m_surfacePixels->pixels);
	return pixels;
}

Uint32 LTexture::getPitch32() const
{
	Uint32 pitch = 0;
	if (m_surfacePixels)
		pitch = m_surfacePixels->pitch / 4;
	return pitch;
}