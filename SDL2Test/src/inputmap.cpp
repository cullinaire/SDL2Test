#include "inputmap.h"

InputMap::InputMap()
{
	playerInput[0] = MOVE_LEFT;
}

//Define a game input by inputting a keyboard code to associate with it
//The playerInput index location corresponds to the numeric value of the
//scancode according to the SDL_Scancode enum
int InputMap::DefineInput(SDL_Scancode scancode, gameInput desiredInput)
{
	playerInput[scancode] = desiredInput;
	return scancode;
}

//Simply returns the gameInput value stored at the particular index corresponding
//to the keyboard scancode
gameInput InputMap::returnInput(SDL_Scancode scancode)
{
	return playerInput[scancode];
}

//Returns a list of human readable form of the gameInput values
//Current implementation is a hack - not sure how to automatically
//convert enum labels to string but something tells me this is a fundamentally
//flawed design. A shame since an enum is so appropriate for this type
//of usage otherwise.
void InputMap::getInputNames(std::list<std::string> *listOfNames)
{
	listOfNames->push_back("move_left");
	listOfNames->push_back("move_right");
	listOfNames->push_back("move_up");
	listOfNames->push_back("move_down");
	listOfNames->push_back("jump");
	listOfNames->push_back("crouch");
	listOfNames->push_back("shoot");
}