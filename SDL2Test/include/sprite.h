#ifndef SPRITE_H_
#define SPRITE_H_

//sprite.h - routines for working with sprite sheets

#include "SDL.h"
#include <string>

//Class Sprite - Just defines a single sprite with a single image loaded as texture
//that is hooked to whatever renderer is active in the window being used.
//This class should only be instantiated as a part of the SpriteSheet class.
class Sprite
{
public:
	Sprite(const std::string bmpfilename, SDL_Renderer *renderer);
	void Draw(SDL_Rect src, SDL_Rect dst);
	~Sprite();
private:
	SDL_Texture *texture;
	SDL_Surface *image;
	SDL_Renderer *rend;	//Just a pointer to external renderer - DO NOT DELETE
};

typedef struct SheetInfo
{
	int id;			//Unique id for cell
	SDL_Rect cell;	//location and size of cell rectangle relative to source texture
} SheetInfo;

//Class SpriteSheet - Takes a Class Sprite object and defines the cells to project from it.
//The cell info is provided at instantiation.
//The info includes: number of cells, location of each cell (x,y,w,h)
//Member function Draw() will intake desired cell id, and output to specified location.
class SpriteSheet
{
public:
	SpriteSheet(const std::string bmpfilename, SDL_Renderer *renderer, SDL_Rect cell, int row, int col, int total);
	void Draw(int id, SDL_Rect dest);
	~SpriteSheet();
private:
	Sprite *source;
	SheetInfo *sheetinfo;
};

#endif