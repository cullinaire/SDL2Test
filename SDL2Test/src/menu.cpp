#include "menu.h"

Menu::Menu()
{
	currentLocation = items.begin();
	index = 0;
	cursor.assign(">");
}

void Menu::OutputMenu(int x, int y)
{
	int cursorX = x;
	int cursorY = y;

	for(itr = items.begin();itr != items.end();++itr)
	{
		//if(itr == currentLocation)	//draw selection cursor if currentLocation points to item
	}
}

void Menu::MoveCursor(bool dir)
{
	//If dir is true, move forwards, else move backwards
	if(dir)
	{
		//items->end() is a tail that does not point to any of the elements, so the -- is needed
		if(currentLocation == --items.end())
		{
			currentLocation = items.begin();
			index = 0;
		}
		else
		{
			++currentLocation;
			++index;
		}
	}
	else
	{
		//Similarly, currentLocation needs to wrap to one element before items->end()
		if(currentLocation != items.begin())
		{
			--currentLocation;
			--index;
		}
		else
		{
			currentLocation = --items.end();
			index = items.size() - 1;
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