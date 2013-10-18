#include "txtlayer.h"

TxtLayer::TxtLayer(BMPText *defaultFont)
{
	e_defaultFont = defaultFont;
	for(int i=0;i < TXTLAYER_CAPACITY;++i)
	{
		textList[i].font = e_defaultFont;
		textList[i].empty = true;
	}
}

void TxtLayer::ReceiveString(std::string text, int x, int y, BMPText *font)
{
	for(int i=0;i < TXTLAYER_CAPACITY;++i)
	{
		if(textList[i].empty == true)
		{
			textList[i].userString = text;
			textList[i].font = font;
			textList[i].x = x;
			textList[i].y = y;
			textList[i].empty = false;
			break;
		}
	}
}

void TxtLayer::ReceiveString(std::string text, SDL_Rect dest)
{
	for(int i=0;i < TXTLAYER_CAPACITY;++i)
	{
		if(textList[i].empty == true)
		{
			textList[i].userString = text;
			textList[i].x = dest.x;
			textList[i].y = dest.y;
			textList[i].empty = false;
			break;
		}
	}
}

void TxtLayer::Clear()
{
	for(int i=0;i < TXTLAYER_CAPACITY;++i)
	{
		textList[i].font = e_defaultFont;
		textList[i].empty = true;
	}
}

void TxtLayer::OutputFrame(SDL_Renderer *rend)
{
	for(int i=0;i < TXTLAYER_CAPACITY;++i)
	{
		if(textList[i].empty == false)
		{
			textList[i].font->PrintText(textList[i].userString,
				textList[i].x, textList[i].y);
		}
	}
}