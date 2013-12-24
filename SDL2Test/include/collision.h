#ifndef COLLISION_H_
#define COLLISION_H_

#ifdef __linux
	#include "SDL2/SDL.h"
#elif _WIN32
	#include "SDL.h"
#endif
#define MAX_ENCOUNTERS	256
#define MAXAABBS	256
//collision.h - structs and routines to support collision detection
//collision code adapted from http://cokane.com/shmupdevarchive/index.php/topic,1635.0.html
#include <string>
#include <iostream>

typedef enum objType
{
	PLAYER,
	PIE
} objType;

typedef struct AABB
{
	int objID;	//Index in box array
	objType type;	//Identity of the owner
	double vals[2][2];	//corners of the AABB

	// vals[0][0] = minX
    // vals[0][1] = minY
    // vals[1][0] = maxX
    // vals[1][1] = maxY
} AABB;

typedef struct Encounter
{
	int objIDs[2];	//The ids of the objects which are colliding
} Encounter;

typedef struct Endpoint
{
	int type;	// 0 = startPoint (min extreme), 1 = endPoint (max extreme)
	int boxId;	// id of the box this endpoint belongs to
} Endpoint;

class SweepAndPrune
{
public:
	SweepAndPrune();
	~SweepAndPrune();
	void Update();
	void ResolveEncounters(std::string *collmsg);

	void AddEncounter(int objIdA, int objIdB);
    void RemoveEncounter(int objIdA, int objIdB);

	int AddBox(objType type, double minX, double maxX, double minY, double maxY);
	void RemoveBox(int boxId);
	void UpdateBox(int boxId, objType type, double minX, double maxX, double minY, double maxY);

	void drawBoundingBoxes(SDL_Renderer *rend);

private:
	Encounter encounters[MAX_ENCOUNTERS];	//A store of encounters this frame
	int possibleDupes[MAX_ENCOUNTERS];	//scratch pad for duplicate finding
	AABB boxes[MAXAABBS];	//The active AABBs
	Endpoint endpoints[2*MAXAABBS][2];	//Endpoint arrays -> [2 endpts per box] per [axis]

	int numEncounters;	//current number of active encounters
	int numBoxes;	//current number of AABBs to deal with
};

bool collide(const AABB boxA, const AABB boxB);
void drawBoundingBox(SDL_Renderer *rend, AABB box);

#endif