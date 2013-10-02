#ifndef GFXTEXT_H_
#define GFXTEXT_H_

//gfxtext.h - routines for printing bitmapped text strings

#include "sprite.h"
#include "SDL.h"
#include <string>
#include <map>

//Class BMPText - takes a font sprite sheet and translates input strings into appropriate pieces
//of the sprite sheet. Pretty simple.
class BMPText
{
public:
	BMPText(SpriteSheet *font);
	void PrintText(std::string text, int x, int y, SDL_Rect charsize);
private:
	SpriteSheet *source;	//Do not delete, this points to external object
	std::map<char, int> sheetIndex;
};

#endif