#ifndef INPUTCFG_H_
#define INPUTCFG_H_

//inputcfg.h - routines that interactively configure game inputs

#include "inputmap.h"
#include "gfxtext.h"
#include "sprite.h"
#include "menu.h"
#include "SDL.h"
#include <string>
#include <list>

class InputCfg
{
public:
	InputCfg(InputMap *playerMap, SDL_Renderer *rend);
	void startConfig();
	~InputCfg();
private:
	Menu *inputMenu;
	std::list<std::string> inputList;
};

#endif