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
	//Specify a textsheet for drawing as well as a list of strings for the menu items themselves
	Menu(BMPText *extrn_TextWriter, std::list<std::string> *menuitems); 
	void DisplayMenu(int x, int y);
	void MoveCursor(bool dir);
	//Simply return the index of the item being selected so that the calling object can do something
	//specific in response.
	int ExecuteItem();
private:
	std::list<std::string> *items;	//DO NOT delete - instantiated elsewhere
	std::list<std::string>::iterator itr;
	std::list<std::string>::iterator currentLocation;	//location of selection cursor
	int index;	//index to be returned by ExecuteItem();
	BMPText *textWriter;	//DO NOT delete - instantiated elsewhere
};	

#endif