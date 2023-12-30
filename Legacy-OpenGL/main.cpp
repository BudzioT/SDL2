#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "GL/GL.h"
#include "GL/GLU.h"

#include <stdio.h>
#include <string>
#include <fstream>


/* Initialize the program */
bool init();

/* Initialize matrices and clear color */
bool initGL();

/* Input handler */
void handleKeys(unsigned char key, int x, int y);

/* Per frame update */
void update();

/* Render quad to screen */
void render();

/* Free media and shut down SDL */
void close();


/* Screen dimensions */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/* Global window */
SDL_Window* window = nullptr;

/* OpenGL context */
SDL_GLContext context;

/* Render flag */
bool renderQuad = true;


int main(int argc, char* args[])
{
	/* Initialize */
	if (!init()) {
		printf("Failed to initialize!\n");
		close();
		return -1;
	}


	bool quit = false;
	SDL_Event e;

	/* Enable text input */
	SDL_StartTextInput();

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			/* Handle keypresses with current mouse position */
			else if (e.type == SDL_TEXTINPUT) {
				int x = 0, y = 0;
				SDL_GetMouseState(&x, &y);
				handleKeys(e.text.text[0], x, y);
			}
		}

		/* Render quad */
		render();

		/* Update screen */
		SDL_GL_SwapWindow(window);
	}

	/* Disable text input */
	SDL_StopTextInput();

	/* Clean up */
	close();
	return 0;
}


bool init()
{
	bool success = true;

	/* Initialize video */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL couldn't initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	/* Use OpenGL 2.1 */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	/* Create window */
	window = SDL_CreateWindow("SDL Particle Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) {
		printf("Couldn't create window! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		/* Create context */
		context = SDL_GL_CreateContext(window);
		if (!context) {
			printf("Couldn't create OpenGL context! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			/* Use VSync */
			if (SDL_GL_SetSwapInterval(1) < 0)
				printf("Warning: Unable to set VSync! SDL_Error: %s\n", SDL_GetError());

			/* Initialize OpenGL */
			if (!initGL()) {
				printf("Couldn't initialize OpenGL!\n");
				success = false;
			}
		}
	}

	return success;
}


bool initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	/* Initialize projection matrix */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* Check for error */
	error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	/* Initialize Modelview matrix */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/* Check for error */
	error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	/* Initialize clear color */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	/* Check for error */
	error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	return success;
}

void handleKeys(unsigned char key, int x, int y)
{
	/* Toggle quad */
	if (key == 'q')
		renderQuad = !renderQuad;
}

void update()
{
	/* No per frame update needed */
}

void render()
{
	/* Clear color buffer */
	glClear(GL_COLOR_BUFFER_BIT);

	/* Render quad */
	if (renderQuad) {
		glBegin(GL_QUADS);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f( 0.5f, -0.5f);
			glVertex2f( 0.5f,  0.5f);
			glVertex2f(-0.5f,  0.5f);
		glEnd();
	}
}

void close()
{

	/* Destroy windows */
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	/* Quit the SDL library */
	SDL_Quit();
}