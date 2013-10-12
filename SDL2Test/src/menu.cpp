#include "menu.h"

Menu::Menu(TxtLayer *txtOut)
{
	cursor = items.begin();
	cursorText.assign(">");
	output = txtOut;
	//index = 0;
	//numItems = 0;
}

void Menu::InsertItem(std::string item, int index, BMPText *p_font)
{
	menuItem element;
	element.text = item;
	element.index = index;
	element.font = p_font;
	items.push_back(element);
	cursor = items.begin();
	//++numItems;
}

void Menu::OutputMenu(int x, int y)
{
	int menuX = x;
	int menuY = y;

	for(itr = items.begin();itr != items.end();++itr)
	{
		output->ReceiveString(itr->text, menuX, menuY, itr->font->getCharSize(), itr->font);
		if(itr == cursor)	//draw selection cursor if currentLocation points to item
		{
			output->ReceiveString(cursorText, menuX - itr->font->getCharSize().w,
				menuY, itr->font->getCharSize(), itr->font);
		}
		menuY += itr->font->getCharSize().h;	//Go to next line
	}
}

void Menu::MoveCursor(bool dir)
{
	//If dir is true, move forwards, else move backwards
	if(dir)
	{
		//items->end() is a tail that does not point to any of the elements, so the -- is needed
		if(cursor == --items.end())
		{
			cursor = items.begin();
			//index = 0;
		}
		else
		{
			++cursor;
			//++index;
		}
	}
	else
	//Moving backwards
	{
		//Similarly, itr needs to wrap to one element before items->end()
		if(cursor != items.begin())
		{
			--cursor;
			//--index;
		}
		else
		//Moving backwards from beginning of list, wrap to end of list
		{
			cursor = --items.end();
			//index = numItems;
		}
	}
}

int Menu::ExecuteItem()
{
	return cursor->index;
	//cursor->itemFunc(index);
}