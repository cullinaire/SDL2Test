#ifndef MENU_H_
#define MENU_H_

//menu.h - manages menu creation, display, and interaction
//Each menu is a list of strings. Determination of what code to execute is done by
//reading the integer index of the menu item.

#include "gfxtext.h"
#include <list>
#include <string>

class Menu
{
public:
	Menu(BMPText *extrn_TextWriter);
	void DisplayMenu(int x, int y);
	void MoveCursor(bool dir);
	void ExecuteItem();
private:
	std::list<std::string> rootmenu;
	std::list<std::string>::iterator itr;
	std::list<std::string>::iterator currentLocation;	//location of selection cursor
	BMPText *textWriter;	//DO NOT delete - instantiated elsewhere
};	

#endif