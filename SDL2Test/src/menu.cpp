#include "menu.h"

Menu::Menu(BMPText *extrn_TextWriter)
{
	textWriter = extrn_TextWriter;
	rootmenu.push_front("first item");
	rootmenu.push_front("second item");
	rootmenu.push_front("last item");
	currentLocation = rootmenu.begin();
}

void Menu::DisplayMenu(int x, int y)
{
	int cursorX = x;
	int cursorY = y;

	for(itr = rootmenu.begin();itr != rootmenu.end();++itr)
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
		//rootmenu.end() is a tail that does not point to any of the elements, so the -- is needed
		if(currentLocation == --rootmenu.end())
			currentLocation = rootmenu.begin();
		else
			++currentLocation;
	}
	else
	{
		//Similarly, currentLocation needs to wrap to one element before rootmenu.end()
		if(currentLocation != rootmenu.begin())
			--currentLocation;
		else
			currentLocation = --rootmenu.end();
	}
}

void Menu::ExecuteItem()
{
	if(*currentLocation == "first item")
		rootmenu.push_front("pushed first yeah");
	else
		rootmenu.push_front("pushed ssomething else yea yea");
}