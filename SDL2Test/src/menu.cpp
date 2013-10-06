#include "menu.h"

Menu::Menu(BMPText *extrn_TextWriter, std::list<std::string> *menuitems)
{
	textWriter = extrn_TextWriter;
	items = menuitems;
	currentLocation = items->begin();
	index = 0;
}

void Menu::DisplayMenu(int x, int y)
{
	int cursorX = x;
	int cursorY = y;

	for(itr = items->begin();itr != items->end();++itr)
	{
		textWriter->PrintText(*itr, cursorX, cursorY, textWriter->getCharSize());
		if(itr == currentLocation)	//draw selection cursor if currentLocation points to item
			textWriter->PrintText(">", cursorX-8, cursorY, textWriter->getCharSize());
		cursorY += textWriter->getCharSize().h;
	}
}

void Menu::MoveCursor(bool dir)
{
	//If dir is true, move forwards, else move backwards
	if(dir)
	{
		//items->end() is a tail that does not point to any of the elements, so the -- is needed
		if(currentLocation == --items->end())
		{
			currentLocation = items->begin();
			index = 0;
		}
		else
		{
			++currentLocation;
			++index;
		}
	}
	else
	//Moving backwards
	{
		//Similarly, currentLocation needs to wrap to one element before items->end()
		if(currentLocation != items->begin())
		{
			--currentLocation;
			--index;
		}
		else
		//Moving backwards from beginning of list, wrap to end of list
		{
			currentLocation = --items->end();	//Note that the actual items->end() does not point to the last element
			index = items->size() - 1;
		}
	}
}

int Menu::ExecuteItem()
{
	/*if(*currentLocation == "first item")
		items->push_front("pushed first yeah");
	else
		items->push_front("pushed ssomething else yea yea");*/
	return index;
}