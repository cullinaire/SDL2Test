#ifndef PLAYER_H_
#define PLAYER_H_

//player.h - encapsulates everything a player needs to do
//Each player will receive input. In this case just input device button
//down/up events. These will be translated into actions based on the
//inputmap defined for that player, and sent to the various animobjs attached
//to the player.
//The physics calculations including collision detection will be done outside
//of this; only the initial values are set in this object such as direction
//and acceleration.

#ifdef __linux
	#include "SDL2/SDL.h"
#elif _WIN32
	#include "SDL.h"
#endif
#include <string>
#include "inputmap.h"
#include "inputcfg.h"
#include "animobj.h"
#include "txtlayer.h"

typedef enum PlayerState
{
	ALIVE,
	DEAD
} PlayerState;

class Player
{
public:
	Player(AnimObj *p_animobj, InputCfg *p_inputCfg, int p_id);
	void configInput(SDL_Scancode lastKey, bool *waitingForInput, bool *menuactive);
	void assignInput(SDL_Scancode lastKey);
	std::string getInputName(SDL_Scancode lastKey);
	void processKeyDown(SDL_Scancode p_scancode, bool *keyPressed);
	void processKeyUp(SDL_Scancode p_scancode, bool *keyPressed);
	void emitInfo(TxtLayer *txtOut);
	void updatePhys();
private:
	int playerID;
	PlayerState state;
	InputMap keyMap;
	InputCfg *e_inputCfg;	//Do not delete - the e_ prefix denotes external object
	AnimObj *e_animobj;
	int xpos;
	int ypos;
	int xvel;	//Velocity is pixels per whatever timestep we choose to use
	int yvel;

	//Debug information to screen stuff - non essential
	std::string animInfo;
	std::string leftKeyInfo;
	std::string rightKeyInfo;
	std::string velInfo;
	SDL_Rect animDest;
	SDL_Rect leftKeyDest;
	SDL_Rect rightKeyDest;
	SDL_Rect velDest;
};

#endif