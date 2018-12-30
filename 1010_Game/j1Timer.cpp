// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
j1Timer::j1Timer(bool run)
{
	if (run)
		Start();
	else {
		running = false;
		started_at = 0;
		stopped_at = 0;
	}
}

// ---------------------------------------------
void j1Timer::Start()
{
	started_at = SDL_GetTicks();
	running = true;
}

float j1Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
	return ReadSec();
}

void j1Timer::Reset()
{
	running = false;
	started_at = 0;
	stopped_at = 0;
}

// ---------------------------------------------
uint32 j1Timer::Read() const
{
	if (running)
	{
		return SDL_GetTicks() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}

// ---------------------------------------------
float j1Timer::ReadSec() const
{
	return float (Read()) / 1000.0f;
}

bool j1Timer::IsRunning()
{
	return running;
}
