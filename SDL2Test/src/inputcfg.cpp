#include "inputcfg.h"

InputCfg::InputCfg(InputMap *p_playerMap, SDL_Renderer *rend, TxtLayer *txtOut, BMPText *bmpFont)
{
	playerMap = p_playerMap;
	textOutput = txtOut;
	inputMenu = new Menu(textOutput);
	font = bmpFont;
	currentInput = UNDEFINED;

	inputMenu->InsertItem("left", 0, bmpFont);
	inputMenu->InsertItem("right", 1, bmpFont);
	inputMenu->InsertItem("up", 2, bmpFont);
	inputMenu->InsertItem("down", 3, bmpFont);
	inputMenu->InsertItem("jump", 4, bmpFont);
	inputMenu->InsertItem("crouch", 5, bmpFont);
	inputMenu->InsertItem("shoot", 6, bmpFont);
}

void InputCfg::showMenu()
{
	inputMenu->OutputMenu(64, 64);
}

void InputCfg::showStatus()
{
	SDL_Rect fontDim;
	fontDim.w = fontDim.h = 8;

	textOutput->ReceiveString(statusMsg, 128, 64, fontDim, font);
}

void InputCfg::menuDown()
{
	inputMenu->MoveCursor(true);
}

void InputCfg::menuUp()
{
	inputMenu->MoveCursor(false);
}

void InputCfg::menuSelect()
{
	switch(inputMenu->ExecuteItem())
	{
	case 0:
		statusMsg.clear();
		statusMsg.assign("press a key to assign to : ");
		statusMsg.append(playerMap->returnInputName(MOVE_LEFT));
		currentInput = MOVE_LEFT;
		break;
	case 1:
		statusMsg.clear();
		statusMsg.assign("press a key to assign to : ");
		statusMsg.append(playerMap->returnInputName(MOVE_RIGHT));
		currentInput = MOVE_RIGHT;
		break;
	case 2:
		statusMsg.clear();
		statusMsg.assign("press a key to assign to : ");
		statusMsg.append(playerMap->returnInputName(MOVE_UP));
		currentInput = MOVE_UP;
		break;
	case 4:
		statusMsg.clear();
		statusMsg.assign("press a key to assign to : ");
		statusMsg.append(playerMap->returnInputName(MOVE_DOWN));
		currentInput = MOVE_DOWN;
		break;
	}
}

void InputCfg::assignInput(SDL_Scancode scancode)
{
	if(currentInput != UNDEFINED)
	{
		playerMap->DefineInput(scancode, currentInput);
		statusMsg.clear();
		statusMsg.assign(playerMap->returnInputName(currentInput));
		statusMsg.append(" has been assigned to key: ");
		statusMsg.append(SDL_GetScancodeName(scancode));
		currentInput = UNDEFINED;
	}
	else
	{
		statusMsg.clear();
		statusMsg.assign("error: currentinput is undefined!!");
	}
}

InputCfg::~InputCfg()
{
	delete inputMenu;
}