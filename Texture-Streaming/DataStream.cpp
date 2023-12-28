#include "DataStream.h"
#include "SDL2/SDL_image.h"

#include <sstream>

DataStream::DataStream()
{
	m_images[0] = NULL;
	m_images[1] = NULL;
	m_images[2] = NULL;
	m_images[3] = NULL;

	m_currentImage = 0;
	m_delayFrames = 4;
}

bool DataStream::loadMedia()
{
	bool success = true;

	for (int i = 0; i < 4; ++i)
	{
		std::stringstream path;
		path << "Images/foo_walk_" << i << ".png";

		SDL_Surface* loadedSurface = IMG_Load(path.str().c_str());
		if (loadedSurface == NULL)
		{
			printf("Unable to load %s! SDL_image error: %s\n", path.str().c_str(), IMG_GetError());
			success = false;
		}
		else
		{
			m_images[i] = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0);
		}

		SDL_FreeSurface(loadedSurface);
	}

	return success;
}

void DataStream::free()
{
	for (int i = 0; i < 4; ++i)
	{
		SDL_FreeSurface(m_images[i]);
		m_images[i] = NULL;
	}
}

void* DataStream::getBuffer()
{
	--m_delayFrames;
	if (m_delayFrames == 0)
	{
		++m_currentImage;
		m_delayFrames = 4;
	}

	if (m_currentImage == 4)
	{
		m_currentImage = 0;
	}

	return m_images[m_currentImage]->pixels;
}