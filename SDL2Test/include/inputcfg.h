#ifndef INPUTCFG_H_
#define INPUTCFG_H_

//inputcfg.h - routines that interactively configure game inputs

#include "inputmap.h"
#ifdef __linux
	#include "SDL2/SDL.h"
#elif _WIN32
	#include "SDL.h"
#endif
#include "txtlayer.h"
#include "gfxtext.h"
#include "sprite.h"
#include "menu.h"
#include <string>
#include <list>

typedef struct assignedInput
{
	SDL_Scancode scancode;
	gameInput associatedInput;
} assignedInput;

class InputCfg
{
public:
	InputCfg(InputMap *p_playerMap, SDL_Renderer *rend, TxtLayer *txtOut, BMPText *bmpFont);
	void registerQuit(bool *quit);
	void showMenu();
	void showStatus();
	void menuDown();
	void menuUp();
	void menuSelect();
	void assignInput(SDL_Scancode scancode);
	~InputCfg();
private:
	Menu *inputMenu;
	Menu *statusMenu;
	InputMap *playerMap;	//Do not delete - instantiated elsewhere
	std::string statusMsg;
	TxtLayer *textOutput;	//Do not delete - instantiated elsewhere
	BMPText *font;			//Do not delete - instantiated elsewhere
	gameInput currentInput;
	std::list<assignedInput> alreadyAssigned;
	std::list<assignedInput>::iterator itr;
	int currentIndex;
	bool *quit;				//Do not delete - instantiated elsewhere
};

#endif
