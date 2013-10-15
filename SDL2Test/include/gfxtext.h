#ifndef GFXTEXT_H_
#define GFXTEXT_H_

//gfxtext.h - routines for printing bitmapped text strings

#ifdef __linux
	#include "SDL2/SDL.h"
#elif _WIN32
	#include "SDL.h"
#endif
#include "sprite.h"
#include <string>
#include <map>

//Class BMPText - takes a font sprite sheet and translates input strings into appropriate pieces
//of the sprite sheet. Pretty simple.
class BMPText
{
public:
	BMPText(SpriteSheet *font);
	SDL_Rect getCharSize();
	void ChangeFont(SpriteSheet *newfont);
	void PrintText(const std::string &text, int x, int y, SDL_Rect charsize);
private:
	SpriteSheet *source;	//Do not delete, this points to external object
	std::map<char, int> sheetIndex;
};

#endif
