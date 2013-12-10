#include "menu.h"

Menu::Menu(TxtLayer *txtOut)
{
	cursor = items.begin();
	cursorText.assign(">");
	output = txtOut;
	title.clear();
}

void Menu::SetTitle(std::string title)
{
	this->title.clear();
	this->title = title;
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

void Menu::ReplaceItem(std::string replacement, int index, BMPText *p_font)
{
	itr = items.begin();
	for(int i = 0;i < index;++i)
		++itr;
	itr->text = replacement;
	itr->font = p_font;
}

void Menu::OutputMenu(int x, int y)
{
	int menuX = x;
	int menuY = y;

	output->ReceiveString(title, menuX, menuY, items.begin()->font);	//menu title

	menuY += 2*(items.begin()->font->getCharSize().h);

	for(itr = items.begin();itr != items.end();++itr)
	{
		output->ReceiveString(itr->text, menuX, menuY, itr->font);
		if(itr == cursor)	//draw selection cursor if currentLocation points to item
		{
			output->ReceiveString(cursorText, menuX - itr->font->getCharSize().w,
				menuY, itr->font);
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

void Menu::DefineCursor(std::string cText)
{
	cursorText = cText;
}

int Menu::ExecuteItem()
{
	return cursor->index;
	//cursor->itemFunc(index);
}