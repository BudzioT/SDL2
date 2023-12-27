#include "LWindow.h"

#include <sstream>


LWindow::LWindow() : m_window(nullptr), m_mouseFocus(false), m_keyboardFocus(false), 
m_fullscreen(false), m_minimized(false), m_width(0), m_height(0)
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
	}
	
	return m_window;
}

SDL_Renderer* LWindow::createRenderer()
{
	return SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void LWindow::handleEvent(SDL_Event& event)
{
	/* Window event occured */
	if (event.type == SDL_WINDOWEVENT) {
		/* Caption update flag */
		bool updateCaption = false;

		switch (event.window.event) {
			/* On window size change get new dimensions and repaint */
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			m_width = event.window.data1;
			m_height = event.window.data2;
			SDL_RenderPresent(renderer);
			break;

			/* On exposure repaint */
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(renderer);
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
		}

		/* Update window caption with new data */
		if (updateCaption) {
			std::stringstream caption;
			caption << "SDL Tutorial - MouseFocus: " << ((m_mouseFocus) ? "On" : "Off")
				<< ", KeyboardFocus: " << ((m_keyboardFocus) ? "On" : "Off");
			SDL_SetWindowTitle(m_window, caption.str().c_str());
		}
		/* Enter exit fullscreen on return key */
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
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