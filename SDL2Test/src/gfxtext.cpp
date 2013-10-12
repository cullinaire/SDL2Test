#include "gfxtext.h"

BMPText::BMPText(SpriteSheet *font)
{
	source = font;
	//Associate various characters with spritesheet-specific indices
	sheetIndex['!'] = 1;
	sheetIndex['"'] = 2;
	sheetIndex['#'] = 3;
	sheetIndex['$'] = 4;
	sheetIndex['%'] = 5;
	sheetIndex['&'] = 6;
	sheetIndex['\''] = 7;
	sheetIndex['('] = 8;
	sheetIndex[')'] = 9;
	sheetIndex['*'] = 10;
	sheetIndex['+'] = 11;
	sheetIndex[','] = 12;
	sheetIndex['-'] = 13;
	sheetIndex['.'] = 14;
	sheetIndex['/'] = 15;
	sheetIndex[':'] = 26;
	sheetIndex[';'] = 27;
	sheetIndex['<'] = 28;
	sheetIndex['='] = 29;
	sheetIndex['>'] = 30;
	sheetIndex['?'] = 31;
	sheetIndex['@'] = 32;
	sheetIndex['['] = 59;
	sheetIndex['\\'] = 60;	//Just like windows, no backslash with the japanese font. Yen sign.
	sheetIndex[']'] = 61;
	sheetIndex['^'] = 62;
	sheetIndex['_'] = 63;
	sheetIndex['{'] = 91;
	sheetIndex['}'] = 93;
	sheetIndex['~'] = 94;
}

SDL_Rect BMPText::getCharSize()
{
	return source->getCellSize();
}

void BMPText::ChangeFont(SpriteSheet *newfont)
{
	source = newfont;
}

void BMPText::PrintText(const std::string &text, int x, int y, SDL_Rect charsize)
{
	SDL_Rect srcRect;	//determines the beginning of the text, as well as spacing
	srcRect.x = x;
	srcRect.y = y;
	srcRect.w = charsize.w;
	srcRect.h = charsize.h;

	//decoding which char is which will obviously depend on sprite sheet used. Basically the offset
	//of the ABC... from the beginning of the file will need to be determined in advance.
	//'A' starts at index 33 for this particular sheet, and '0' starts at 16
	//ASCII lowercase 'a' starts at 97 and '0' starts at '48'
	//As it turns out, you don't even need to cast the characters to int! Not sure if this is portable though.
	for(unsigned int i=0;i < text.length();++i)
	{
		if(text[i] == ' ')
			source->Draw(0, srcRect);
		else if(text[i] >= 48 && text[i] <= 57)	//is a number
			source->Draw(text[i]-32, srcRect);
		else if(text[i] >= 97 && text[i] <= 122) //is a lowercase letter
			source->Draw(text[i]-64, srcRect);
		else if(text[i] >= 65 && text[i] <= 90)	//is an uppercase letter
			source->Draw(text[i]-32, srcRect);	//current fontsheet only has one case so use same
		else
			source->Draw(sheetIndex[text[i]], srcRect);

		srcRect.x += srcRect.w;	//advance print location
	}
}