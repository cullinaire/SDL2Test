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

#define LEFT_RUN_SPD	-100
#define RIGHT_RUN_SPD	100
#define STOPPED_SPD		0

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

typedef struct PlayerState
{
	bool alive;
	bool facingleft;
	bool airborne;
} PlayerState;

typedef struct State
{
	double x;	//pos
	double y;	//pos
	double xv;	//vel
	double yv;	//vel
} State;

typedef struct Derivative
{
	double dx, dy;		//derivative of pos: vel
	double dxv, dyv;	//derivative of vel: acc
} Derivative;

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
	void Integrate(double t, double dt);
	void Interpolate(const double alpha);
private:
	int playerID;
	PlayerState playerState;
	InputMap keyMap;
	InputCfg *e_inputCfg;	//Do not delete - the e_ prefix denotes external object
	AnimObj *e_animobj;
	State playerPosVel;
	State prevState;
	State renderState;
	void accel(const State &state, double t, Derivative &p_output);
	Derivative eval(const State &initial, double t, double dt, const Derivative &d);
	Derivative eval(const State &initial, double t);

	//Debug information to screen stuff - non essential
	std::string animInfo;
	std::string leftKeyInfo;
	std::string rightKeyInfo;
	std::string pposInfo;
	std::string derivInfo;
	SDL_Rect animDest;
	SDL_Rect leftKeyDest;
	SDL_Rect rightKeyDest;
	SDL_Rect pposDest;
	SDL_Rect derivDest;
};

#endif