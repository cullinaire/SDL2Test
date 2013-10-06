#include "inputmap.h"

InputMap::InputMap()
{
	for(int i=0;i < MAXINPUTS;++i)
		playerInput[i] = UNDEFINED;	//The default state
}

void InputMap::ClearMap()
{
	for(int i=0;i < MAXINPUTS;++i)
		playerInput[i] = UNDEFINED;	//The default state
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