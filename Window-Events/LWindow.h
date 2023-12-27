#pragma once
#include "SDL2/SDL.h"

/* Global window dimensions */
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

/* Global renderer */
extern SDL_Renderer* renderer;

class LWindow
{
public:
	LWindow();
	~LWindow();

	/* Create window */
	bool init();
	/* Create renderer from internal window */
	SDL_Renderer* createRenderer();
	/* Deallocate internals */
	void free();
	/* Handle window events */
	void handleEvent(SDL_Event& event);
	/* Window dimensions */
	int getWidth() const;
	int getHeight() const;
	/* Window focii */
	bool hasMouseFocus() const;
	bool hasKeyboardFocus() const;
	bool isMinimized() const;

private:
	/* Window data */
	SDL_Window* m_window;
	/* Window dimensions */
	int m_width;
	int m_height;
	/* Window focus */
	bool m_mouseFocus;
	bool m_keyboardFocus;
	bool m_fullscreen;
	bool m_minimized;
};

