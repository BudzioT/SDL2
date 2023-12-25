#pragma once
#include "SDL2/SDL.h"

/* The application time based timer */
class LTimer
{
public:
	LTimer();

	/* Various clock actions */
	void start();
	void stop();
	void pause();
	void unpause();

	/* Get the timer's time */
	Uint32 getTicks() const;

	/* Check the timer's status */
	bool isStarted() const;
	bool isPaused() const;

private:
	/* The clock time when the timer started */
	Uint32 m_startTicks;
	/* The ticks stored when the timer was paused */
	Uint32 m_pausedTicks;
	/* The timer status */
	bool m_paused;
	bool m_started;
};