#include "timekeep.h"

void UpdateTime(Uint64 &counter, Uint64 &lastSample)
{
	Uint64 currentTime = SDL_GetPerformanceCounter();
	Uint64 diff = currentTime - lastSample;
	counter += diff;
	lastSample = currentTime;
}