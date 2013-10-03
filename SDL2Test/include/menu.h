#ifndef MENU_H_
#define MENU_H_

//menu.h - manages menu creation, display, and interaction
//Each menu is a list of strings. Selection is done by walking the list.
//Eventually, this menu class will be instantiated with the ingredients for a given
//menu by the object that owns the menu instance. Then ExecuteItem would just return the appropriate
//index and the owning object could react based on that value.

#include "gfxtext.h"
#include <list>
#include <string>

class Menu
{
public:
	Menu(BMPText *extrn_TextWriter);
	void DisplayMenu(int x, int y);
	void MoveCursor(bool dir);
	void ExecuteItem();	//Eventually will be returning an int or something so the owning object can use that.
private:
	std::list<std::string> rootmenu;
	std::list<std::string>::iterator itr;
	std::list<std::string>::iterator currentLocation;	//location of selection cursor
	BMPText *textWriter;	//DO NOT delete - instantiated elsewhere
};	

#endif