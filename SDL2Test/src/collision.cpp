#include "collision.h"

bool collide(const AABB boxA, const AABB boxB)
{
	if(boxA.vals[1][0] < boxB.vals[0][0]) { return false; }
    if(boxA.vals[0][0] > boxB.vals[1][0]) { return false; }
    if(boxA.vals[1][1] < boxB.vals[0][1]) { return false; }
    if(boxA.vals[0][1] > boxB.vals[1][1]) { return false; }
    return true;
}

void drawBoundingBox(SDL_Renderer *rend, AABB box)
{
	SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);	//Set AABB if visible to red
	SDL_Point corners[5];
	//upper left corner
	corners[0].x = box.vals[0][0];	//minX
	corners[0].y = box.vals[0][1];	//minY

	//lower left corner
	corners[1].x = box.vals[0][0];	//minX
	corners[1].y = box.vals[1][1];	//maxY

	//lower right corner
	corners[2].x = box.vals[1][0];	//maxX
	corners[2].y = box.vals[1][1];	//maxY

	//upper right corner
	corners[3].x = box.vals[1][0];	//maxX
	corners[3].y = box.vals[0][1];	//minY

	//upper left corner
	corners[4].x = box.vals[0][0];	//minX
	corners[4].y = box.vals[0][1];	//minY

	SDL_RenderDrawLines(rend, corners, 5);
}

void SweepAndPrune::drawBoundingBoxes(SDL_Renderer *rend)
{
	SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);	//Set AABB if visible to red
	SDL_Point corners[5];

	for(int i=0;i < MAXAABBS;++i)
	{
		if(boxes[i].objID != -1)
		{
			//upper left corner
			corners[0].x = boxes[i].vals[0][0];	//minX
			corners[0].y = boxes[i].vals[0][1];	//minY

			//lower left corner
			corners[1].x = boxes[i].vals[0][0];	//minX
			corners[1].y = boxes[i].vals[1][1];	//maxY

			//lower right corner
			corners[2].x = boxes[i].vals[1][0];	//maxX
			corners[2].y = boxes[i].vals[1][1];	//maxY

			//upper right corner
			corners[3].x = boxes[i].vals[1][0];	//maxX
			corners[3].y = boxes[i].vals[0][1];	//minY

			//upper left corner
			corners[4].x = boxes[i].vals[0][0];	//minX
			corners[4].y = boxes[i].vals[0][1];	//minY

			SDL_RenderDrawLines(rend, corners, 5);
		}
	}
}

SweepAndPrune::SweepAndPrune()
{
	numBoxes = 0;
	numEncounters = 0;

	for(int i=0;i < MAXAABBS;++i)
	{
		boxes[i].objID = -1;	//This marks the array cell as empty
	}

	for(int i=0;i < MAX_ENCOUNTERS;++i)
	{
		encounters[i].objIDs[0] = -1;	//Same as above, marking array cells as empty
		encounters[i].objIDs[1] = -1;
		//Also sanitize this one just in case
		possibleDupes[i] = 0;
	}

	for(int axis = 0;axis < 2;++axis)
	{
		for(int j=1;j < 2*MAXAABBS;++j)
		{
			endpoints[j][axis].boxId = -1;
		}
	}
}

SweepAndPrune::~SweepAndPrune()
{
}

void SweepAndPrune::Update()
{
	//sort lists on each axis
	for(int axis = 0;axis < 2;++axis)
	{
		//go through each endpoint in turn
		int firstGoodIndex;
		bool fgiFound = false;
		int secondGoodIndex;
		for(int j = 1;j < 2*MAXAABBS;++j)
		{
			if(endpoints[j-1][axis].boxId != -1 && !fgiFound)
			{
				firstGoodIndex = j-1;	//establish the first valid endpoint in array
				fgiFound = true;	//Stop looking for a valid first point once one is found
			}
			else
				continue;	//skip until one is found

			//obviously only start looking for the 2nd pt once 1st is found
			if(endpoints[j][axis].boxId != -1 && fgiFound)
			{
				secondGoodIndex = j;
				break;
			}
			else
				continue;
		}
		
		int i = 0;
		int j = 1;
		
		while(j < 2*MAXAABBS)
		{
			i = firstGoodIndex;
			j = secondGoodIndex;

			int keyType = endpoints[j][axis].type;
			int keyBoxId = endpoints[j][axis].boxId;

			//get the min val if this is a startPoint else get the max val
			double keyVal = boxes[keyBoxId].vals[keyType][axis];

			//compare the keyval to the value one before it in the array (our comparison value) and
			//swap places if need be. Keep doing this until no more swaps are needed or until we
			//reach the start of the array.
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
				endpoints[j][axis].type = compType;
				endpoints[i][axis].type = keyType;

				endpoints[j][axis].boxId = compBoxId;
				endpoints[i][axis].boxId = keyBoxId;

				//we must decrement i so that we continue searching down the array
				while(i >= 0)
				{
					--i;
					if(endpoints[i][axis].boxId != -1)
						break;
				}
			}

			//find the next valid secondGoodIndex, if any
			firstGoodIndex = secondGoodIndex;	//"increment" firstGoodIndex to next valid endpoint
			++j;
			while(j < 2*MAXAABBS)
			{
				if(endpoints[j][axis].boxId != -1)
				{
					secondGoodIndex = j;
					break;
				}
				else
					++j;
			}
		}
	}
}

void SweepAndPrune::ResolveEncounters(std::string *collmsg)
{
	collmsg->assign("No collisions right now");
    // Iterate through your encounter list and trigger collision resolution code
    // for each pair of objects in there
	for(int i=0;i < MAX_ENCOUNTERS;++i)
	{
		if(encounters[i].objIDs[0] != -1)
		{
			collmsg->assign("Collision detected");
		}
	}
}


void SweepAndPrune::AddEncounter(int objIdA, int objIdB)
{
    // Add encounter between the inputted objects to the list
    // being careful not to duplicate existing ecounters

	//first look for duplicates. This step marks down possible points of duplication
	int numActualDupes = 0;
	int numPossibleDupes = 0;
	for(int i=0;i < MAX_ENCOUNTERS;++i)
	{
		if(encounters[i].objIDs[0] == objIdA || encounters[i].objIDs[0] == objIdB)
		{
			possibleDupes[numPossibleDupes] = i;
			++numPossibleDupes;
		}
	}

	//Next we check each of these points to see if any are dupes
	for(int i=0;i < numPossibleDupes;++i)
	{
		//The reason the following check is valid is that the 0 index of objIDs was already marked as one
		//of the two parameter IDs. Since it is impossible (better check) to have an encounter with the same
		//ID repeated, the other ID in index 1 must be the other parameter ID. Further, only indexes with
		//one of the parameters is checked so there is no danger of marking a valid encounter as a dupe
		//(i.e. an encounter with index 0 being a totally different object)
		if(encounters[possibleDupes[i]].objIDs[1] == objIdA || encounters[possibleDupes[i]].objIDs[1] == objIdB)
		{
			++numActualDupes;
		}
	}

	if(numActualDupes >= 1)
	{
		//Don't add encounter!...in fact if the number is GREATER than 1, there is a problem and might want
		//to raise an exception...
	}
	else
	{
		//No dupes so add the encounter already in the first empty slot
		for(int i=0;i < MAX_ENCOUNTERS;++i)
		{
			if(encounters[i].objIDs[0] == -1)
			{
				encounters[i].objIDs[0] = objIdA;
				encounters[i].objIDs[1] = objIdB;
			}
		}
	}
}


void SweepAndPrune::RemoveEncounter(int objIdA, int objIdB)
{
    // Remove encounter between the inputted objects from the
    // list if it exists (order of parameters do not matter)
	// Worst case scenario will iterate all elements of encounters array once
	for(int i=0;i < MAX_ENCOUNTERS;++i)
	{
		if(encounters[i].objIDs[0] == objIdA)
		{
			for(int j = i;j < MAX_ENCOUNTERS;++j)
			{
				if(encounters[j].objIDs[1] == objIdB)
				{
					encounters[j].objIDs[0] = -1;
					encounters[j].objIDs[1] = -1;
					break;	//The assumption is that there will be only one encounter between two unique objects
				}
			}
		}
		else if(encounters[i].objIDs[0] == objIdB)
		{
			for(int j = i;j < MAX_ENCOUNTERS;++j)
			{
				if(encounters[j].objIDs[1] == objIdA)
				{
					encounters[j].objIDs[0] = -1;
					encounters[j].objIDs[1] = -1;
					break;	//The assumption is that there will be only one encounter between two unique objects
				}
			}
		}
	}
}

int SweepAndPrune::AddBox(objType type, double minX, double minY, double maxX, double maxY)
{
	int boxID = -1;
    // add box
	for(int i=0;i < MAXAABBS;++i)
	{
		if(boxes[i].objID == -1)	//Find the first empty cell
		{
			boxes[i].objID = i;
			boxes[i].type = type;
			boxes[i].vals[0][0] = minX;
			boxes[i].vals[0][1] = minY;
			boxes[i].vals[1][0] = maxX;
			boxes[i].vals[1][1] = maxY;

			// add endpoints
			endpoints[2*i][0].type = 0;		//xmin
			endpoints[2*i][0].boxId = i;
			endpoints[2*i+1][0].type = 1;	//xmax
			endpoints[2*i+1][0].boxId = i;
			endpoints[2*i][1].type = 0;		//ymin
			endpoints[2*i][1].boxId = i;
			endpoints[2*i+1][1].type = 1;	//ymax
			endpoints[2*i+1][1].boxId = i;

			boxID = i;
			break;	//forgot this earlier...
		}
	}

    // note that its not important to insert these in their correct sorted value position
    // as I havn't since the next call to Update() will sort that out for us.

    ++numBoxes;

    // return the id of this box so that objects know which box to update when they move
    return boxID;
}


void SweepAndPrune::RemoveBox(int boxId)
{
    // Remove the box with the corresponding id being careful to remove all its
    // endpoints as well as any encounters relating to this boxes object
	boxes[boxId].objID = -1;
	endpoints[2*boxId][0].boxId = -1;
	endpoints[2*boxId+1][0].boxId = -1;
	endpoints[2*boxId][1].boxId = -1;
	endpoints[2*boxId+1][1].boxId = -1;

	for(int i=0;i < MAX_ENCOUNTERS;++i)
	{
		if(encounters[i].objIDs[0] == boxId)
		{
			this->RemoveEncounter(boxId, encounters[i].objIDs[1]);
		}
		else if(encounters[i].objIDs[1] == boxId)
		{
			this->RemoveEncounter(boxId, encounters[i].objIDs[0]);
		}
	}

	--numBoxes;
}

void SweepAndPrune::UpdateBox(int boxId, objType type, double minX, double minY, double maxX, double maxY)
{
    // Replace the vals of the box with the inputted boxId
    // Whenever an object moves it should call this to update its collision box(es)
    // Each object will know which boxId to pass in since it should store the boxId value
    // returned when AddBox() is called.
	boxes[boxId].type = type;
	boxes[boxId].vals[0][0] = minX;
	boxes[boxId].vals[0][1] = minY;
	boxes[boxId].vals[1][0] = maxX;
	boxes[boxId].vals[1][1] = maxY;
}