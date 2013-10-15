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
	//Clear the old key-input mapping, if any
	for(int i=0;i < MAXINPUTS;++i)
	{
		if(playerInput[i] == desiredInput)
			playerInput[i] = UNDEFINED;
	}
	playerInput[scancode] = desiredInput;
	return scancode;
}

//Simply returns the gameInput value stored at the particular index corresponding
//to the keyboard scancode
gameInput InputMap::returnInput(SDL_Scancode scancode)
{
	return playerInput[scancode];
}

std::string InputMap::returnInputName(gameInput input)
{
	switch(input)
	{
	case UNDEFINED:
		return "undefined";
		break;
	case MOVE_LEFT:
		return "move left";
		break;
	case MOVE_RIGHT:
		return "move right";
		break;
	case MOVE_DOWN:
		return "move down";
		break;
	case MOVE_UP:
		return "move up";
		break;
	case JUMP:
		return "jump";
		break;
	case CROUCH:
		return "crouch";
		break;
	case SHOOT:
		return "shoot";
		break;
	default:
		return "nonono";
		break;
	}
	return "somethingwentwrong";
}