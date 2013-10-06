#ifndef MENU_H_
#define MENU_H_

//menu.h - manages menu creation, display, and interaction
//Each menu is a list of strings. Selection is done by walking the list.
//Eventually, this menu class will be instantiated with the ingredients for a given
//menu by the object that owns the menu instance. Then ExecuteItem would just return the appropriate
//index and the owning object could react based on that value.
//Display is handled by txtlayer.

#include "txtlayer.h"
#include "gfxtext.h"
#include <list>
#include <string>

typedef struct menuItem
{
	std::string text;
	void (*itemFunc) ();
	BMPText *font;
} menuItem;

class Menu
{
public:
	Menu(TxtLayer *txtOut);
	void InsertItem(std::string item, void (*itemFunc) (), BMPText *p_font);
	void OutputMenu(int x, int y);
	void MoveCursor(bool dir);
	void ExecuteItem();
private:
	std::list<menuItem> items;
	std::list<menuItem>::iterator itr;	//Used for looping through list
	std::list<menuItem>::iterator cursor;	//Maintains "current" menu selection
	std::string cursorText;
	TxtLayer *output;	//Do not delete, instantiated elsewhere
};	

#endif