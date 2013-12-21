#include "collision.h"

bool collide(const AABB boxA, const AABB boxB)
{
	if(boxA.vals[1][0] < boxB.vals[0][0]) { return false; }
    if(boxA.vals[0][0] > boxB.vals[1][0]) { return false; }
    if(boxA.vals[1][1] < boxB.vals[0][1]) { return false; }
    if(boxA.vals[0][1] > boxB.vals[1][1]) { return false; }
    return true;
}

void SweepAndPrune::Update()
{
	//sort lists on each axis
	for(int axis = 0;axis < 2;++axis)
	{
		//go through each endpoint in turn
		for(int j = 1;j < numBoxes*2;++j)
		{
			int keyType = endpoints[j][axis].type;
			int keyBoxId = endpoints[j][axis].boxId;

			//get the min val if this is a startPoint else get the max val
			double keyVal = boxes[keyBoxId].vals[keyType][axis];

			//compare the keybal to the value one before it in the array (our comparison value) and
			//swap places if need be. Keep doing this until no more swaps are needed or until we
			//reach the start of the array.
			int i = j-1;
			while(i >= 0)
			{
				//get our comparison value in the same way we got the key value
				int compType = endpoints[i][axis].type;
				int compBoxId = endpoints[i][axis].boxId;
				int compVal = boxes[compBoxId].vals[compType][axis];

				if(compVal < keyVal)
				{
					//these values are in the correct order so break out of this while loop
					break;
				}

				//these vals are swapping places which relates to a change in the state of the scene
				//so update our encounter list accordingly

				//if an endpoint is swapping to the left on a startpoint then we know these objects
				//are leaving contact so remove any encounters relating to these objects
				if((compType == 0) && (keyType == 1))
				{
					RemoveEncounter(compBoxId, keyBoxId);
				}
				else
				{
					//else if a startpoint is swapping to the left of an endpoint, these objects might
					//be colliding
					if((compType == 1) && (keyType == 0))
					{
						//this only tells us that they overlap on one axis
						//to be sure of collision we must do an intersection test
						if(collide(boxes[compBoxId], boxes[keyBoxId]))
						{
							AddEncounter(compBoxId, keyBoxId);	//these AABBs now intersect
						}
					}
				}

				//finally we must swap the points
				endpoints[i+1][axis].type = compType;
				endpoints[i][axis].type = keyType;

				endpoints[i+1][axis].boxId = compBoxId;
				endpoints[i][axis].boxId = keyBoxId;

				//we must decrement i so that we continue searching down the array
				--i;
			}
		}
	}
}

void SweepAndPrune::ResolveEncounters()
{
    // Iterate through your encounter list and trigger collision resolution code
    // for each pair of objects in there
}


void SweepAndPrune::AddEncounter(int objIdA, int objIdB)
{
    // Add encounter between the inputted objects to the list
    // being careful not to duplicate existing ecounters
}


void SweepAndPrune::RemoveEncounter(int objIdA, int objIdB)
{
    // Remove encounter between the inputted objects from the
    // list if it exists
}

int SweepAndPrune::AddBox(int objId, objType type, double minX, double maxX, double minY, double maxY)
{
    // I'll fill this one out since it shows how to set up the various structures

    // add box
    boxes[numBoxes].objID = objId;
    boxes[numBoxes].vals[0][0] = minX;
    boxes[numBoxes].vals[0][1] = minY;
    boxes[numBoxes].vals[1][0] = maxX;
    boxes[numBoxes].vals[1][1] = maxY;

    // add endpoints
    endpoints[2*numBoxes][0].type = 0;
    endpoints[2*numBoxes][0].boxId = numBoxes;
    endpoints[2*numBoxes+1][0].type = 1;
    endpoints[2*numBoxes+1][0].boxId = numBoxes;
    endpoints[2*numBoxes][1].type = 0;
    endpoints[2*numBoxes][1].boxId = numBoxes;
    endpoints[2*numBoxes+1][1].type = 1;
    endpoints[2*numBoxes+1][1].boxId = numBoxes;
    // note that its not important to insert these in their correct sorted value position
    // as I havn't since the next call to Update() will sort that out for us.

    numBoxes += 1;

    // return the id of this box so that objects know which box to update when they move
    return numBoxes-1;
}


void SweepAndPrune::RemoveBox(int boxId)
{
    // Remove the box with the corresponding id being careful to remove all its
    // endpoints as well as any encounters relating to this boxes object
}


void SweepAndPrune::UpdateBox(int boxId, objType type, double minX, double maxX, double minY, double maxY)
{
    // Replace the vals of the box with the inputted boxId
    // Whenever an object moves it should call this to update its collision box(es)
    // Each object will know which boxId to pass in since it should store the boxId value
    // returned when AddBox() is called.
}