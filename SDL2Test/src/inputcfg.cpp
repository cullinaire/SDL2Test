#include "inputcfg.h"

InputCfg::InputCfg(SDL_Renderer *rend, TxtLayer *txtOut, BMPText *bmpFont)
{
	textOutput = txtOut;
	inputMenu = new Menu(textOutput);
	statusMenu = new Menu(textOutput);
	statusMenu->DefineCursor(" ");
	inputMenu->SetTitle("Define key input");
	statusMenu->SetTitle("Key assignments");
	font = bmpFont;
	alreadyAssigned.clear();	//Maybe build this list from an external config file later

	currentInput = UNDEFINED;

	inputMenu->InsertItem("left", 0, 0, bmpFont);
	inputMenu->InsertItem("right", 1, 1, bmpFont);
	inputMenu->InsertItem("up", 2, 2, bmpFont);
	inputMenu->InsertItem("down", 3, 3, bmpFont);
	inputMenu->InsertItem("jump", 4, 4, bmpFont);
	inputMenu->InsertItem("crouch", 5, 5, bmpFont);
	inputMenu->InsertItem("shoot", 6, 6, bmpFont);
	inputMenu->InsertItem("quit", 7, 7, bmpFont);

	statusMenu->InsertItem("left is undefined", 0, 0, bmpFont);
	statusMenu->InsertItem("right is undefined", 1, 1, bmpFont);
	statusMenu->InsertItem("up is undefined", 2, 2, bmpFont);
	statusMenu->InsertItem("down is undefined", 3, 3, bmpFont);
	statusMenu->InsertItem("jump is undefined", 4, 4, bmpFont);
	statusMenu->InsertItem("crouch is undefined", 5, 5, bmpFont);
	statusMenu->InsertItem("shoot is undefined", 6, 6, bmpFont);
}

void InputCfg::assignPlayerMap(InputMap *p_playerMap)
{
	e_playerMap = p_playerMap;
}

void InputCfg::registerQuit(bool *quit)
{
	this->quit = quit;
}

void InputCfg::changeMenuTitle(std::string newTitle)
{
	inputMenu->SetTitle(newTitle);
}

void InputCfg::showMenu()
{
	inputMenu->OutputMenu(64, 96);
}

void InputCfg::showStatus()
{
	textOutput->ReceiveString(statusMsg, 128, 64, font);
	e_playerMap->populateStatus(*statusMenu);
	statusMenu->OutputMenu(256, 256);
}

//Updates currentInput based on the desired Input to assign a key to
void InputCfg::processInput(SDL_Scancode lastKey, bool *waitingForInput, bool *menuactive)
{
	switch(lastKey)
	{
	case SDL_SCANCODE_DOWN:
		inputMenu->MoveCursor(true);
		break;
	case SDL_SCANCODE_UP:
		inputMenu->MoveCursor(false);
		break;
	case SDL_SCANCODE_RETURN:
		currentIndex = inputMenu->ExecuteItem();
		switch(currentIndex)
		{
		case 0:
			statusMsg.assign("press a key to assign to : ");
			statusMsg.append(e_playerMap->returnInputName(MOVE_LEFT));
			currentInput = MOVE_LEFT;
			break;
		case 1:
			statusMsg.assign("press a key to assign to : ");
			statusMsg.append(e_playerMap->returnInputName(MOVE_RIGHT));
			currentInput = MOVE_RIGHT;
			break;
		case 2:
			statusMsg.assign("press a key to assign to : ");
			statusMsg.append(e_playerMap->returnInputName(MOVE_UP));
			currentInput = MOVE_UP;
			break;
		case 3:
			statusMsg.assign("press a key to assign to : ");
			statusMsg.append(e_playerMap->returnInputName(MOVE_DOWN));
			currentInput = MOVE_DOWN;
			break;
		case 4:
			statusMsg.assign("press a key to assign to : ");
			statusMsg.append(e_playerMap->returnInputName(JUMP));
			currentInput = JUMP;
			break;
		case 5:
			statusMsg.assign("press a key to assign to : ");
			statusMsg.append(e_playerMap->returnInputName(CROUCH));
			currentInput = CROUCH;
			break;
		case 6:
			statusMsg.assign("press a key to assign to : ");
			statusMsg.append(e_playerMap->returnInputName(SHOOT));
			currentInput = SHOOT;
			break;
		case 7:
			*quit = true;
			break;
		}

		*waitingForInput = true;
		break;
	case SDL_SCANCODE_ESCAPE:
		if(!*menuactive)
			*menuactive = true;
		else
			*menuactive = false;
		break;
	default:
		break;
	}
}

//assigns scancode to the currentInput at the time
void InputCfg::assignInput(SDL_Scancode scancode)
{
	bool duplicate = false;
	gameInput oldInput;

	if(currentInput != UNDEFINED)
	{
		//search to see if scancode is already assigned
		for(itr = alreadyAssigned.begin();itr != alreadyAssigned.end();++itr)
		{
			if(itr->scancode == scancode)
			{
				duplicate = true;
				break;
			}
		}

		assignedInput newInput;
		newInput.associatedInput = currentInput;
		newInput.scancode = scancode;

		//If input is already assigned, overwrite with new assignment
		if(duplicate)
		{
			oldInput = itr->associatedInput;
			itr->associatedInput = currentInput;
			itr->scancode = scancode;
			e_playerMap->DefineInput(scancode, UNDEFINED);	//unnecessary since the defineinput later on will just overwrite
			statusMsg.assign(e_playerMap->returnInputName(oldInput));
			statusMsg.append(" is undefined");
			statusMenu->ReplaceItem(statusMsg, oldInput, font);	//hopefully the enum will suffice as the index of the old input
		}
		else
		{
			alreadyAssigned.push_back(newInput);
		}

		e_playerMap->DefineInput(scancode, currentInput);
		statusMsg.assign(e_playerMap->returnInputName(currentInput));
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

void InputCfg::assignInput(SDL_Scancode scancode, gameInput p_currentInput)
{
	currentInput = p_currentInput;
	currentIndex = (int)currentInput;	//Needed since currentIndex is normally set by menu selection
	this->assignInput(scancode);
}

InputCfg::~InputCfg()
{
	delete inputMenu;
	delete statusMenu;
}