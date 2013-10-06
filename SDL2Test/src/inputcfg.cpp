#include "inputcfg.h"

InputCfg::InputCfg(InputMap *playerMap, SDL_Renderer *rend)
{
	SDL_Rect fontDim;
	fontDim.w = 8;
	fontDim.h = 8;
	SpriteSheet arcadeFont("../assets/drbrfont.bmp", rend, fontDim, 1, 95, 95);
	BMPText textWriter(&arcadeFont);
	playerMap->getInputNames(&inputList);
	//inputMenu = new Menu(&textWriter, &inputList);
}

InputCfg::~InputCfg()
{
	delete inputMenu;
}