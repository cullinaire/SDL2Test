#ifndef COLLISION_H_
#define COLLISION_H_

#ifdef __linux
	#include "SDL2/SDL.h"
#elif _WIN32
	#include "SDL.h"
#endif

#define MAX_ENCOUNTERS	256
#define MAXAABBS	256
#define MAXENDPT	1
#define MINENDPT	0
#define XAXIS		0
#define	YAXIS		1

//collision.h - structs and routines to support collision detection
//collision code adapted from http://cokane.com/shmupdevarchive/index.php/topic,1635.0.html
//Collision structure: AABBs, Endpoints, and Encounters
//AABBs are the basic axis aligned bounding boxes
//Endpoints are the max and min points in the axes of the box. They are used in the sweep and prune
//routine to determine which AABBs to collide against each other.
//Encounters are active collisions in a given frame.

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

typedef enum objType
{
	PLAYER,
	PIE
} objType;

typedef struct AABB
{
	int boxId;	//Index in box array
	int endptId;	//Index in endpoint array
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
	int boxId;	// id of the box this endpoint belongs to.
				//Remember that this is not the index to the endpoint array since it is sorted and persistent
} Endpoint;

class SweepAndPrune
{
public:
	SweepAndPrune();
	~SweepAndPrune();
	void Update(const AABB box);
	void Add(const AABB box);
	//void ResolveEncounters(std::string *collmsg);

	/*void AddEncounter(int objIdA, int objIdB);
    void RemoveEncounter(int objIdA, int objIdB);*/

	//int AddBox(objType type, double minX, double maxX, double minY, double maxY);
	//void RemoveBox(int boxId);
	//void UpdateBox(int boxId, objType type, double minX, double maxX, double minY, double maxY);

	void drawBoundingBoxes(SDL_Renderer *rend);

private:
	std::vector<Encounter> encounters;	//A store of encounters this frame
	std::vector<int> possibleDupes;	//scratch pad for duplicate finding
	std::vector<AABB> boxes;	//The active AABBs
	std::vector<Endpoint> endpointsX;
	std::vector<Endpoint> endpointsY;

	int numEncounters;	//current number of active encounters
	int numBoxes;	//current number of AABBs to deal with
};

bool collide(const AABB boxA, const AABB boxB);
void drawBoundingBox(SDL_Renderer *rend, AABB box);

#endif