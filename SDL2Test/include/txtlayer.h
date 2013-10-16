#ifndef TXTLAYER_H_
#define TXTLAYER_H_

#define TXTLAYER_CAPACITY 128
//txtlayer.h - routines to compose and output the text onto the renderer

#include "gfxtext.h"
#include <string>

//This class is responsible for collecting, arranging, and drawing all text on screen for each frame
//Naturally, any class that outputs text needs to interface with this class.
//Will I be able to do it?
//The class will receive as input:
//strings to draw, the starting location of each string, font to use for each string
//Since the number of strings received per frame will be arbitrary, the strings will be stored in
//an array in the form of a struct, which will also contain the other attributes
//like location and font.

typedef struct textElement
{
	std::string userString;
	BMPText *font;
	int x;
	int y;
	bool empty;	//If true, textElement is invalid and is not drawn (and can be overwritten)
} textElement;

class TxtLayer
{
public:
	TxtLayer();
	void ReceiveString(std::string text, int x, int y, BMPText *font);
	void Clear();
	void OutputFrame(SDL_Renderer *rend);
private:
	textElement textList[TXTLAYER_CAPACITY];
};

#endif