#include "inputcfg.h"

InputCfg::InputCfg(InputMap *p_playerMap, SDL_Renderer *rend, TxtLayer *txtOut, BMPText *bmpFont)
{
	playerMap = p_playerMap;
	textOutput = txtOut;
	inputMenu = new Menu(textOutput);
	statusMenu = new Menu(textOutput);
	statusMenu->DefineCursor(" ");
	font = bmpFont;
	currentInput = UNDEFINED;

	inputMenu->InsertItem("left", 0, bmpFont);
	inputMenu->InsertItem("right", 1, bmpFont);
	inputMenu->InsertItem("up", 2, bmpFont);
	inputMenu->InsertItem("down", 3, bmpFont);
	inputMenu->InsertItem("jump", 4, bmpFont);
	inputMenu->InsertItem("crouch", 5, bmpFont);
	inputMenu->InsertItem("shoot", 6, bmpFont);
	inputMenu->InsertItem("quit", 7, bmpFont);

	statusMenu->InsertItem("left:", 0, bmpFont);
	statusMenu->InsertItem("right:", 1, bmpFont);
	statusMenu->InsertItem("up:", 2, bmpFont);
	statusMenu->InsertItem("down:", 3, bmpFont);
	statusMenu->InsertItem("jump:", 4, bmpFont);
	statusMenu->InsertItem("crouch:", 5, bmpFont);
	statusMenu->InsertItem("shoot:", 6, bmpFont);
}

void InputCfg::registerQuit(bool *quit)
{
	this->quit = quit;
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
	statusMenu->OutputMenu(256, 256);
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
	currentIndex = inputMenu->ExecuteItem();
	switch(currentIndex)
	{
	case 0:
		statusMsg.assign("press a key to assign to : ");
		statusMsg.append(playerMap->returnInputName(MOVE_LEFT));
		currentInput = MOVE_LEFT;
		break;
	case 1:
		statusMsg.assign("press a key to assign to : ");
		statusMsg.append(playerMap->returnInputName(MOVE_RIGHT));
		currentInput = MOVE_RIGHT;
		break;
	case 2:
		statusMsg.assign("press a key to assign to : ");
		statusMsg.append(playerMap->returnInputName(MOVE_UP));
		currentInput = MOVE_UP;
		break;
	case 3:
		statusMsg.assign("press a key to assign to : ");
		statusMsg.append(playerMap->returnInputName(MOVE_DOWN));
		currentInput = MOVE_DOWN;
		break;
	case 4:
		statusMsg.assign("press a key to assign to : ");
		statusMsg.append(playerMap->returnInputName(JUMP));
		currentInput = JUMP;
		break;
	case 5:
		statusMsg.assign("press a key to assign to : ");
		statusMsg.append(playerMap->returnInputName(CROUCH));
		currentInput = CROUCH;
		break;
	case 6:
		statusMsg.assign("press a key to assign to : ");
		statusMsg.append(playerMap->returnInputName(SHOOT));
		currentInput = SHOOT;
		break;
	case 7:
		*quit = true;
		break;
	}
}

void InputCfg::assignInput(SDL_Scancode scancode)
{
	if(currentInput != UNDEFINED)
	{
		playerMap->DefineInput(scancode, currentInput);
		statusMsg.assign(playerMap->returnInputName(currentInput));
		statusMsg.append(" has been assigned to key: ");
		statusMsg.append(SDL_GetScancodeName(scancode));
		statusMenu->ReplaceItem(statusMsg, currentIndex, font);
		currentInput = UNDEFINED;
	}
	else
	{
		statusMsg.assign("error: currentinput is undefined!!");
	}
}

InputCfg::~InputCfg()
{
	delete inputMenu;
	delete statusMenu;
}