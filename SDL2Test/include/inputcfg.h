#ifndef INPUTCFG_H_
#define INPUTCFG_H_

//inputcfg.h - routines that interactively configure game inputs

#include "inputmap.h"
#include "txtlayer.h"
#include "gfxtext.h"
#include "sprite.h"
#include "menu.h"
#include "SDL.h"
#include <string>
#include <list>

class InputCfg
{
public:
	InputCfg(InputMap *p_playerMap, SDL_Renderer *rend, TxtLayer *txtOut, BMPText *bmpFont);
	void showMenu();
	void showStatus();
	void menuDown();
	void menuUp();
	void menuSelect();
	void assignInput(SDL_Scancode scancode);
	~InputCfg();
private:
	Menu *inputMenu;
	InputMap *playerMap;	//Do not delete - instantiated elsewhere
	std::string statusMsg;
	TxtLayer *textOutput;	//Do not delete - instantiated elsewhere
	BMPText *font;			//Do not delete - instantiated elsewhere
	gameInput currentInput;
};

#endif