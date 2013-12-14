#ifndef COLLISION_H_
#define COLLISION_H_

//collision.h - structs and routines to support collision detection

typedef struct AABB
{
	int objID;	//Owner of this particular AABB
	double vals[2][2];	//corners of the AABB

	// vals[0][0] = minX
    // vals[0][1] = minY
    // vals[1][0] = maxX
    // vals[1][1] = maxY
} AABB;

#endif