#include "Timer.h"
#include "Log.h"

#include "SDL_timer.h"

Timer::Timer()
{
	start = 0;
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
	return SDL_GetTicks() - start;
}

float Timer::ReadSec()
{
	return (SDL_GetTicks() - start) / 1000.0f;
}