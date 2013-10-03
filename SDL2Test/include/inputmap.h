#ifndef INPUTMAP_H_
#define INPUTMAP_H_

//inputmap.h - input mapper routines
//Maps physical keys and buttons to internal game functions
//Mapping will be done with dual arrays. One array for keys and the other array for
//corresponding actions.
//Since the ordering of the array containing the supported keys/buttons/actions will
//be known in advance, there is no need for a map or anything like that.

#include "SDL.h"

typedef enum

class InputMap
{
public:
	InputMap();
	void DefineInput();

	~InputMap();
private:
}



#endif