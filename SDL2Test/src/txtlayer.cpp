#include "txtlayer.h"

TxtLayer::TxtLayer()
{
	for(int i=0;i < TXTLAYER_CAPACITY;++i)
	{
		textList[i].empty = true;
	}
}

int TxtLayer::ReceiveString(std::string text, int x, int y, SDL_Rect charsize, BMPText *font)
{
	for(int i=0;i < TXTLAYER_CAPACITY;++i)
	{
		if(textList[i].empty = true)
		{
			textList[i].userString = text;
			textList[i].font = font;
			textList[i].x = x;
			textList[i].y = y;
			textList[i].charsize = charsize;
			textList[i].empty = false;
			return i;
		}
	}
	return -1;	//this should not happen, time to expand array size!
}

void TxtLayer::RemoveString(int index)
{
	textList[index].empty = true;
}

void TxtLayer::OutputFrame(SDL_Renderer *rend)
{
	for(int i=0;i < TXTLAYER_CAPACITY;++i)
	{
		if(textList[i].empty == false)
		{
			textList[i].font->PrintText(textList[i].userString,
				textList[i].x, textList[i].y, textList[i].charsize);
		}
	}
}