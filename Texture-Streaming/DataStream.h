#pragma once
#include "SDL2/SDL.h"

/* Test animation stream */
class DataStream
{
public:
	DataStream();

	/* Load initial media */
	bool loadMedia();

	/* Deallocate */
	void free();

	/* Get current frame data */
	void* getBuffer();

private:
	/* Internal data */
	SDL_Surface* m_images[4];
	int m_currentImage;
	int m_delayFrames;
};