#ifndef COLLISION_H_
#define COLLISION_H_

#define MAX_ENCOUNTERS	256
#define MAXAABBS	256
//collision.h - structs and routines to support collision detection
//collision code adapted from http://cokane.com/shmupdevarchive/index.php/topic,1635.0.html
//#include <vector>

typedef enum objType
{
	PLAYER,
	PIE
} objType;

typedef struct AABB
{
	int objID;	//Owner of this particular AABB
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

typedef struct SweepAndPrune
{
	Encounter encounters[MAX_ENCOUNTERS];	//A store of encounters this frame
	AABB boxes[MAXAABBS];	//The active AABBs
	Endpoint endpoints[2*MAXAABBS][2];	//Endpoint arrays -> [2 endpts per box] per [axis]

	int numEncounters;	//current number of active encounters
	int numBoxes;	//current number of AABBs to deal with

	void Update();
	void ResolveEncounters();

	void AddEncounter(int objIdA, int objIdB);
    void RemoveEncounter(int objIdA, int objIdB);

	int AddBox(int objId, objType type, double minX, double maxX, double minY, double maxY);
	void RemoveBox(int boxId);
	void UpdateBox(int boxId, objType type, double minX, double maxX, double minY, double maxY);
} SweepAndPrune;

//class Collider
//{
//public:
//	Collider();
//	void Update();
//	void InsertBox(const AABB newBox);
//	void RemoveBox(const int id);
//private:
//	std::vector<AABB> boxesToConsider[MAXAABBS];
//};

bool collide(const AABB boxA, const AABB boxB);

#endif