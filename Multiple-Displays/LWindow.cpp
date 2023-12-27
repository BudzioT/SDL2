#include "LWindow.h"

#include <sstream>


LWindow::LWindow() : m_window(nullptr), m_renderer(nullptr), m_windowID(-1), m_windowDisplayID(-1),
m_mouseFocus(false), m_keyboardFocus(false), m_fullscreen(false), m_minimized(false), m_shown(false), 
m_width(0), m_height(0)
{
}

LWindow::~LWindow()
{
	free();
}

bool LWindow::init()
{
	/* Create window */
	m_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (m_window) {
		m_mouseFocus = true;
		m_keyboardFocus = true;
		m_width = SCREEN_WIDTH;
		m_height = SCREEN_HEIGHT;

		/* Create renderer for window */
		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!m_renderer) {
			printf("Couldn't create window renderer! SDL_Error: %s\n", SDL_GetError());
			free();
		}
		else {
			/* Initialize renderer color */
			SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

			/* Grab window IDs */
			m_windowID = SDL_GetWindowID(m_window);
			m_windowDisplayID = SDL_GetWindowDisplayIndex(m_window);

			/* Flag as opened */
			m_shown = true;
		}
	}
	else
		printf("Couldn't create the window! SDL_Error: %s\n", SDL_GetError());
	
	return m_window && m_renderer;
}

void LWindow::handleEvent(SDL_Event& event)
{
	/* Caption update flag */
	bool updateCaption = false;

	/* Window event occured for this window */
	if (event.type == SDL_WINDOWEVENT && event.window.windowID == m_windowID) {
		switch (event.window.event) {
			/* Window moved */
		case SDL_WINDOWEVENT_MOVED:
			m_windowDisplayID = SDL_GetWindowDisplayIndex(m_window);
			updateCaption = true;
			break;

			/* Window appeared */
		case SDL_WINDOWEVENT_SHOWN:
			m_shown = true;
			break;

			/* Window dissapeared */
		case SDL_WINDOWEVENT_HIDDEN:
			m_shown = false;
			break;

			/* On window size change get new dimensions and repaint */
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			m_width = event.window.data1;
			m_height = event.window.data2;
			SDL_RenderPresent(m_renderer);
			break;

			/* On exposure repaint */
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(m_renderer);
			break;

			/* Mouse entered window */
		case SDL_WINDOWEVENT_ENTER:
			m_mouseFocus = true;
			updateCaption = true;
			break;

			/* Mouse left window */
		case SDL_WINDOWEVENT_LEAVE:
			m_mouseFocus = false;
			updateCaption = true;
			break;

			/* Window has keyboard focus */
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			m_keyboardFocus = true;
			updateCaption = true;
			break;

			/* Window lost keyboard focus */
		case SDL_WINDOWEVENT_FOCUS_LOST:
			m_keyboardFocus = false;
			updateCaption = true;
			break;

			/* Window minimized */
		case SDL_WINDOWEVENT_MINIMIZED:
			m_minimized = true;
			break;

			/* Window maximized */
		case SDL_WINDOWEVENT_MAXIMIZED:
			m_minimized = false;
			break;

			/* Window restored */
		case SDL_WINDOWEVENT_RESTORED:
			m_minimized = false;
			break;

			/* Hide on close */
		case SDL_WINDOWEVENT_CLOSE:
			SDL_HideWindow(m_window);
			m_shown = false;
			break;
		}
		/* Enter exit fullscreen on return key */
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
			if (m_fullscreen) {
				SDL_SetWindowFullscreen(m_window, 0);
				m_fullscreen = false;
			}
			else {
				SDL_SetWindowFullscreen(m_window, 1);
				m_fullscreen = true;
				m_minimized = false;
			}
		}
	}

	else if (event.type == SDL_KEYDOWN) {
		/* Display change flag */
		bool switchDisplay = false;

		/* Cycle through displays on up/down */
		switch (event.key.keysym.sym) {
		case SDLK_UP:
			++m_windowDisplayID;
			switchDisplay = true;
			break;

		case SDLK_DOWN:
			--m_windowDisplayID;
			switchDisplay = true;
			break;
		}

		/* Display needs to be updated */
		if (switchDisplay) {
			/* Bound display index */
			if (m_windowDisplayID < 0)
				m_windowDisplayID = totalDisplays - 1;
			else if (m_windowDisplayID >= totalDisplays)
				m_windowDisplayID = 0;

			/* Move window to center of next display */
			SDL_SetWindowPosition(m_window, displayBounds[m_windowDisplayID].x
				+ (displayBounds[m_windowDisplayID].w - m_width) / 2,
				displayBounds[m_windowDisplayID].y + (displayBounds[m_windowDisplayID].h - m_height) / 2);
			updateCaption = true;
		}
	}

	/* Update window caption with new data */
	if (updateCaption) {
		std::stringstream caption;
		caption << "SDL Tutorial - ID: " << m_windowID << ", Display: " << m_windowDisplayID 
			<< " MouseFocus: " << ((m_mouseFocus) ? "On" : "Off")
			<< ", KeyboardFocus: " << ((m_keyboardFocus) ? "On" : "Off");
		SDL_SetWindowTitle(m_window, caption.str().c_str());
	}
}

int LWindow::getWidth() const
{
	return m_width;
}

int LWindow::getHeight() const
{
	return m_height;
}

bool LWindow::hasMouseFocus() const
{
	return m_mouseFocus;
}

bool LWindow::hasKeyboardFocus() const
{
	return m_keyboardFocus;
}

bool LWindow::isMinimized() const
{
	return m_minimized;
}

bool LWindow::isShown() const
{
	return m_shown;
}

void LWindow::free()
{
	if (m_window) {
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}

	m_minimized = false;
	m_fullscreen = false;
	m_height = 0;
	m_width = 0;
	m_keyboardFocus = false;
	m_mouseFocus = false;
}

void LWindow::focus()
{
	/* Restore window if needed */
	if (!m_shown)
		SDL_ShowWindow(m_window);

	/* Move window forward */
	SDL_RaiseWindow(m_window);
}

void LWindow::render()
{
	if (!m_minimized) {
		/* Clear screen */
		SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(m_renderer);

		/* Update screen */
		SDL_RenderPresent(m_renderer);
	}
}