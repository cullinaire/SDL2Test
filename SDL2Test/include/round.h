//round.h - Wrapper class that represents a single level
//The hope is by wrapping collision and players into a single class, data passing between the two
//will become easier.
//A round has: Players, Collider, Sprites, Animations, TextLayers, Environments, inputcfgs
#ifndef ROUND_H_
#define ROUND_H_

#ifdef __linux
	#include "SDL2/SDL.h"
#elif _WIN32
	#include "SDL.h"
#endif

#include "player.h"
#include "collision.h"

class Round
{
public:
	Round();
	~Round();
	void init();
	void update();
	void render();
private:
	SweepAndPrune collider;
	PlayerGroup players;
};

#endif