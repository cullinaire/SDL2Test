#ifndef TIMEKEEP_H_
#define TIMEKEEP_H_

//timekeep.h - timekeeping routines

#ifdef __linux
	#include "SDL2/SDL.h"
#elif _WIN32
	#include "SDL.h"
#endif

//Updates time counter given by parameter
//Usually the time counter starts at 0, and counts up from there
//Since the counter is a 64bit int, it will be able to maintain a
//non wrapping count for more than a year even with microsecond precision
//lastSample must be in the form of the output from SDL_GetPerformanceCounter()
//The conversion into actual seconds is only done outside the function to
//preserve the stability of the counter over a long period
void UpdateTime(Uint64 &counter, Uint64 &lastSample);

#endif