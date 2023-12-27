#pragma once
#include "SDL2/SDL.h"

/* Global window dimensions */
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

/* Global display informations */
extern int totalDisplays;
extern SDL_Rect* displayBounds;

const int TOTAL_WINDOWS = 1;

class LWindow
{
public:
	LWindow();
	~LWindow();

	/* Create window */
	bool init();

	/* Deallocate internals */
	void free();

	/* Show windows contents */
	void render();

	/* Focus on window */
	void focus();

	/* Handle window events */
	void handleEvent(SDL_Event& event);

	/* Window dimensions */
	int getWidth() const;
	int getHeight() const;

	/* Window focii */
	bool hasMouseFocus() const;
	bool hasKeyboardFocus() const;
	bool isMinimized() const;
	bool isShown() const;

private:
	/* Window data */
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	int m_windowID;
	int m_windowDisplayID;

	/* Window dimensions */
	int m_width;
	int m_height;

	/* Window focus */
	bool m_mouseFocus;
	bool m_keyboardFocus;
	bool m_fullscreen;
	bool m_minimized;
	bool m_shown;
};

