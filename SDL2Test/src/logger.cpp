#ifdef __linux
	#include "SDL2/SDL.h"
#elif __WIN32
	#include "SDL.h"
#endif
#include "logger.h"

void logSDLError(std::ostream &os, const std::string &msg)
{
	os << msg << " error: " << SDL_GetError() << std::endl;
}
