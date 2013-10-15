#ifndef INPUTMAP_H_
#define INPUTMAP_H_

#define MAXINPUTS 256
//inputmap.h - input mapper routines
//Maps physical keys and buttons to internal game functions
//Mapping will be done with dual arrays. One array for keys and the other array for
//corresponding actions.
//Since the ordering of the array containing the supported keys/buttons/actions will
//be known in advance, there is no need for a map or anything like that.

#ifdef __linux
	#include "SDL2/SDL.h"
#elif _WIN32
	#include "SDL.h"
#endif
#include <string>
#include <list>

typedef enum gameInput
{
	UNDEFINED = -1,	//Needed to assign -1 to preserve the indices of the remaining members for various purposes
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	JUMP,
	CROUCH,
	SHOOT
} gameInput;

class InputMap
{
public:
	InputMap();
	void ClearMap();
	int DefineInput(SDL_Scancode scancode, gameInput desiredInput);
	gameInput returnInput(SDL_Scancode scancode);
	std::string returnInputName(gameInput input);
private:
	gameInput playerInput[MAXINPUTS];
};

#endif
