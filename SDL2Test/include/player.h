//player.h - encapsulates everything a player needs to do
//Each player will receive input. In this case just input device button
//down/up events. These will be translated into actions based on the
//inputmap defined for that player, and sent to the various animobjs attached
//to the player.

#ifndef PLAYER_H_
#define PLAYER_H_

#define PLAYER_INITIAL_X	32
#define PLAYER_INITIAL_Y	32
#define DEF_FORCE			70
#define DEF_FRIC			0.95
#define EMPTY_PLAYER		-1
#define MAXPLAYERS			8

#ifdef __linux
	#include "SDL2/SDL.h"
#elif _WIN32
	#include "SDL.h"
#endif

#include <string>
#include <cstdlib>
#include <random>
#include <chrono>
#include "inputmap.h"
#include "inputcfg.h"
#include "animobj.h"
#include "txtlayer.h"
#include "collision.h"
#include "cml\cml.h"

typedef struct PlayerState
{
	bool alive;
	bool moving;
	bool leftpressed;
	bool rightpressed;
	bool uppressed;
	bool downpressed;
} PlayerState;

typedef struct State
{
	cml::vector3d pos;
	cml::vector3d vel;
	cml::vector3d acc;

	double mass;	//in kg
} State;

class Player
{
public:
	Player();
	Player(int p_id);
	Player(SpriteSheet *playerSheet, InputCfg *p_inputCfg, int p_id);
	void relocate(int x, int y);
	void configInput(SDL_Scancode lastKey, bool *waitingForInput, bool *menuactive);
	void assignInput(SDL_Scancode lastKey);
	void assignInput(const std::string keybindFilename);
	std::string getInputName(SDL_Scancode lastKey);
	void processKeyDown(SDL_Scancode p_scancode, bool *keyPressed);
	void processKeyUp(SDL_Scancode p_scancode, bool *keyPressed);
	void modifyForces(double t);
	void verlet(double dt);
	void Interpolate(const double alpha);
	void SelectAnim(double t);
	int getPid();
	void setBoxId(int newId);
	int getBoxId();
	void reportForce(std::string &forceStr);
	void applyImpulse(double duration, cml::vector3d direction, double magnitude);
	AABB outputAABB();

private:
	int playerID;
	double elapsed;
	double impDuration;	//in s
	bool impulseActive;
	cml::vector3d impulse;
	cml::vector3d moveForce;
	cml::vector3d finalForce;
	cml::vector3d dampForce;
	PlayerState playerState;
	InputMap keyMap;
	InputCfg *e_inputCfg;	//Do not delete - the e_ prefix denotes external object
	AnimObj playerAnim;
	AABB playerBox;	//AABB for player collision
	void updateAABB(int x, int y);
	State pstate;
	State previous;
};

class PlayerGroup
{
public:
	PlayerGroup();
	~PlayerGroup();
	bool Add(int id, SweepAndPrune &collider, SpriteSheet &playerSheet, InputCfg &playerCfg);
	void Remove(int id, SweepAndPrune &collider);
	void Render(double alpha, double t);
	void Update(double t, double dt, SweepAndPrune &collider, std::string &forceStr);
	void AssignInput(int id, const std::string defPath);
	bool PlayerExists(int id);
	bool is_Full();
	bool is_Empty();
	void ProcessInput(bool DownElseUp, SDL_Scancode scancode, bool *keyPressed);
	void RandomImpulse();
private:
	int numActive;
	Player *players[MAXPLAYERS];
	Player emptyPlayer;
};

#endif