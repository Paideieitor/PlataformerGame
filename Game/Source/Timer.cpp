#include "Timer.h"
#include "Log.h"

#include "SDL_timer.h"

Timer::Timer(int offset) : start(SDL_GetTicks() - offset), pausedAt(0)
{
}

Timer::~Timer()
{

}

void Timer::Start()
{
	start = SDL_GetTicks();
}

int Timer::Read()
{
	if(pausedAt != 0)
		return pausedAt;
	return SDL_GetTicks() - start;
}

float Timer::ReadSec()
{
	if(pausedAt != 0)
		return pausedAt / 1000.0f;
	return (SDL_GetTicks() - start) / 1000.0f;
}

void Timer::Pause()
{
	if(pausedAt == 0)
		pausedAt = Read();
}

void Timer::Play()
{
	if(pausedAt != 0)
	{
		start = SDL_GetTicks() - pausedAt;
		pausedAt = 0;
	}
}
