#ifndef INPUTCFG_H_
#define INPUTCFG_H_

//inputcfg.h - routines that interactively configure game inputs

#ifdef __linux
	#include "SDL2/SDL.h"
#elif _WIN32
	#include "SDL.h"
#endif
#include "txtlayer.h"
#include "inputmap.h"
#include "gfxtext.h"
#include "sprite.h"
#include "menu.h"
#include <string>
#include <list>
#include <fstream>
#include <iostream>

typedef struct assignedInput
{
	SDL_Scancode scancode;
	gameInput associatedInput;
} assignedInput;

class InputCfg
{
public:
	InputCfg(SDL_Renderer *rend, TxtLayer *txtOut, BMPText *bmpFont);
	void assignPlayerMap(InputMap *p_playerMap);
	void changeMenuTitle(std::string newTitle);
	void showMenu();
	void showStatus();
	void processInput(SDL_Scancode lastKey, bool *waitingForInput, bool *menuactive);
	void assignInput(SDL_Scancode scancode);
	void assignInput(SDL_Scancode scancode, gameInput p_currentInput);
	void assignInput(const std::string bindFilename);
	~InputCfg();
private:
	Menu *inputMenu;
	Menu *statusMenu;
	InputMap *e_playerMap;	//Do not delete - instantiated elsewhere
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
