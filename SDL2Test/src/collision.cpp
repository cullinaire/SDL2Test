#include "collision.h"

bool collide(const AABB boxA, const AABB boxB)
{
	std::cout << "Testing collision..." << std::endl;
	std::cout << "AminX: " << boxA.vals[0][0] <<
		" AminY: " << boxA.vals[0][1] <<
		" AmaxX: " << boxA.vals[1][0] <<
		" AmaxY: " << boxA.vals[1][1] << std::endl;
	std::cout << "BmaxX: " << boxB.vals[1][0] <<
		" BmaxY: " << boxB.vals[1][1] <<
		" BminX: " << boxB.vals[0][0] <<
		" BminY: " << boxB.vals[0][1] << std::endl;
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
		if(boxes[i].boxId != -1)
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

	encounters.resize(MAX_ENCOUNTERS);
	possibleDupes.resize(MAX_ENCOUNTERS);

	boxes.resize(MAXAABBS);

	endpointsX.resize(2*MAXAABBS);
	endpointsY.resize(2*MAXAABBS);

	for(int i=0;i < MAXAABBS;++i)
	{
		boxes[i].boxId = -1;	//This marks the array cell as empty
	}

	for(int i=0;i < MAX_ENCOUNTERS;++i)
	{
		encounters[i].objIDs[0] = -1;	//Same as above, marking array cells as empty
		encounters[i].objIDs[1] = -1;
		//Also sanitize this one just in case
		possibleDupes[i] = 0;
	}

	for(int j=0;j < 2*MAXAABBS;++j)
	{
		endpointsX[j].boxId = -1;
	}
}

SweepAndPrune::~SweepAndPrune()
{
}

void SweepAndPrune::Update(const AABB box)
{
	//Assume that the endpoint array is filled with ... endpoints. The goal is to sort them by
	//their coordinates. The tests for intersection happen each time the sort swaps two endpoints.
	//Assumptions:
	//1. endpoints that belong to the same box obviously will never swap, since they are
	//always in order. So no need to worry about intersecting a box with itself.

	boxes[box.boxId].type = box.type;
	boxes[box.boxId].vals[MINENDPT][XAXIS] = box.vals[MINENDPT][XAXIS];
	boxes[box.boxId].vals[MINENDPT][YAXIS] = box.vals[MINENDPT][YAXIS];
	boxes[box.boxId].vals[MAXENDPT][XAXIS] = box.vals[MAXENDPT][XAXIS];
	boxes[box.boxId].vals[MAXENDPT][YAXIS] = box.vals[MAXENDPT][YAXIS];

	int j = 0;

	//Do X axis stuff first
	for(int keyIdx=1;keyIdx < 2*MAXAABBS;++keyIdx)
	{
		int i = keyIdx;

		while(i > 0)
		{
			//Get key values
			int keyType = endpointsX[i].type;
			int keyId = endpointsX[i].boxId;
			double keyVal = boxes[endpointsX[i].boxId].vals[keyType][XAXIS];	//second 0 is x axis

			j = i-1;
		
			//Get comparison values
			int compType = endpointsX[j].type;
			int compId = endpointsX[j].boxId;
			double compVal = boxes[endpointsX[j].boxId].vals[compType][XAXIS];

			if(keyVal < compVal)	//need to swap at this point
			{
				//Will optimize the following later based on likelihood of occurrence
				if(keyType == MAXENDPT && compType == MAXENDPT)	//just swap, not an indication of collison
				{
					//do nothing
					std::cout << "Max passes Max, doing nothing" << std::endl;
				}
				else if(keyType == MAXENDPT && compType == MINENDPT)	//if MAX is now less than MIN, chances are objects now stop overlapping
				{
					//do something about removing an encounter if necessary
					std::cout << "Max passes min, remove" << std::endl;
				}
				else if(keyType == MINENDPT && compType == MAXENDPT)	//if a box's MIN is less than another's MAX, chances are there is a new overlap
				{
					//investigate further to see if there is an actual collision
					std::cout << "Min passes max, add" << std::endl;
				}
				else if(keyType == MINENDPT && compType == MINENDPT)	//just swap, not an indication of collision
				{
					//do nothing
					std::cout << "Min passes Min, doing nothing" << std::endl;
				}

				//now swap
				endpointsX[i].type = compType;
				endpointsX[i].boxId = compId;

				endpointsX[j].type = keyType;
				endpointsX[j].boxId = keyId;
			}

			//Decrement i to see if further swaps are needed
			--i;
		}
	}
}

void SweepAndPrune::Add(const AABB box)
{
	bool minAdded = false;
	//Add a new box to the collection. Also needs to properly add the endpoints.
	//This is accomplished by calling update()
	for(int i=0;i < MAXAABBS;++i)
	{
		if(boxes[i].boxId == -1)	//found an empty spot
		{
			boxes[i].boxId = i;
			boxes[i].type = box.type;
			boxes[i].vals[MINENDPT][XAXIS] = box.vals[MINENDPT][XAXIS];
			boxes[i].vals[MINENDPT][YAXIS] = box.vals[MINENDPT][YAXIS];
			boxes[i].vals[MAXENDPT][XAXIS] = box.vals[MAXENDPT][XAXIS];
			boxes[i].vals[MAXENDPT][YAXIS] = box.vals[MAXENDPT][YAXIS];

			//Add endpoints - this is a naive implmentation that is very slow
			//Just adding to the tail of the existing list, and relying on update() to
			//bubble them to their final locations
			for(int j=0;j < 2*MAXAABBS;++j)
			{
				if(endpointsX[j].boxId == -1)
				{
					if(minAdded)
					{
						endpointsX[j].boxId = boxes[i].boxId;
						endpointsX[j].type = MAXENDPT;
						break;
					}
					endpointsX[j].boxId = boxes[i].boxId;
					endpointsX[j].type = MINENDPT;
					minAdded = true;
				}
			}

			this->Update(boxes[i]);

			break;
		}
	}
}

//void SweepAndPrune::ResolveEncounters(std::string *collmsg)
//{
//	collmsg->assign("No collisions right now");
//    // Iterate through your encounter list and trigger collision resolution code
//    // for each pair of objects in there
//	for(int i=0;i < MAX_ENCOUNTERS;++i)
//	{
//		if(encounters[i].objIDs[0] != -1)
//		{
//			collmsg->assign("Collision detected");
//		}
//	}
//}


//void SweepAndPrune::AddEncounter(int objIdA, int objIdB)
//{
//    // Add encounter between the inputted objects to the list
//    // being careful not to duplicate existing ecounters
//
//	//first look for duplicates. This step marks down possible points of duplication
//	int numActualDupes = 0;
//	int numPossibleDupes = 0;
//	for(int i=0;i < MAX_ENCOUNTERS;++i)
//	{
//		if(encounters[i].objIDs[0] == objIdA || encounters[i].objIDs[0] == objIdB)
//		{
//			possibleDupes[numPossibleDupes] = i;
//			++numPossibleDupes;
//		}
//	}
//
//	//Next we check each of these points to see if any are dupes
//	for(int i=0;i < numPossibleDupes;++i)
//	{
//		//The reason the following check is valid is that the 0 index of objIDs was already marked as one
//		//of the two parameter IDs. Since it is impossible (better check) to have an encounter with the same
//		//ID repeated, the other ID in index 1 must be the other parameter ID. Further, only indexes with
//		//one of the parameters is checked so there is no danger of marking a valid encounter as a dupe
//		//(i.e. an encounter with index 0 being a totally different object)
//		if(encounters[possibleDupes[i]].objIDs[1] == objIdA || encounters[possibleDupes[i]].objIDs[1] == objIdB)
//		{
//			++numActualDupes;
//		}
//	}
//
//	if(numActualDupes >= 1)
//	{
//		//Don't add encounter!...in fact if the number is GREATER than 1, there is a problem and might want
//		//to raise an exception...
//	}
//	else
//	{
//		//No dupes so add the encounter already in the first empty slot
//		for(int i=0;i < MAX_ENCOUNTERS;++i)
//		{
//			if(encounters[i].objIDs[0] == -1)
//			{
//				encounters[i].objIDs[0] = objIdA;
//				encounters[i].objIDs[1] = objIdB;
//				++numEncounters;
//				break;	//Forgot this again...
//			}
//		}
//	}
//}
//
//
//void SweepAndPrune::RemoveEncounter(int objIdA, int objIdB)
//{
//    // Remove encounter between the inputted objects from the
//    // list if it exists (order of parameters do not matter)
//	// Worst case scenario will iterate all elements of encounters array once
//	for(int i=0;i < MAX_ENCOUNTERS;++i)
//	{
//		if(encounters[i].objIDs[0] == objIdA)
//		{
//			for(int j = i;j < MAX_ENCOUNTERS;++j)
//			{
//				if(encounters[j].objIDs[1] == objIdB)
//				{
//					encounters[j].objIDs[0] = -1;
//					encounters[j].objIDs[1] = -1;
//					--numEncounters;
//					break;	//The assumption is that there will be only one encounter between two unique objects
//				}
//			}
//		}
//		else if(encounters[i].objIDs[0] == objIdB)
//		{
//			for(int j = i;j < MAX_ENCOUNTERS;++j)
//			{
//				if(encounters[j].objIDs[1] == objIdA)
//				{
//					encounters[j].objIDs[0] = -1;
//					encounters[j].objIDs[1] = -1;
//					--numEncounters;
//					break;	//The assumption is that there will be only one encounter between two unique objects
//				}
//			}
//		}
//	}
//}

//int SweepAndPrune::AddBox(objType type, double minX, double minY, double maxX, double maxY)
//{
//	int boxID = -1;
//    // add box
//	for(int i=0;i < MAXAABBS;++i)
//	{
//		if(boxes[i].boxId == -1)	//Find the first empty cell
//		{
//			boxes[i].boxId = i;
//			boxes[i].type = type;
//			boxes[i].vals[0][0] = minX;
//			boxes[i].vals[0][1] = minY;
//			boxes[i].vals[1][0] = maxX;
//			boxes[i].vals[1][1] = maxY;
//
//			// add endpoints
//			endpointsX[2*i].type = 0;		//xmin
//			endpointsX[2*i].boxId = i;
//			endpointsX[2*i].val = minX;
//
//			endpointsX[2*i+1].type = 1;		//xmax
//			endpointsX[2*i+1].boxId = i;
//			endpointsX[2*i+1].val = maxX;
//
//			endpointsY[2*i].type = 0;		//ymin
//			endpointsY[2*i].boxId = i;
//			endpointsY[2*i].val = minY;
//
//			endpointsY[2*i+1].type = 1;		//ymax
//			endpointsY[2*i+1].boxId = i;
//			endpointsY[2*i+1].val = maxY;
//
//			boxID = i;
//			boxes[i].endptId = 2*i;
//			break;	//forgot this earlier...
//		}
//	}
//
//    // note that its not important to insert these in their correct sorted value position
//    // as I havn't since the next call to Update() will sort that out for us.
//
//    ++numBoxes;
//
//    // return the id of this box so that objects know which box to update when they move
//    return boxID;
//}


//void SweepAndPrune::RemoveBox(int boxId)
//{
//    // Remove the box with the corresponding id being careful to remove all its
//    // endpoints as well as any encounters relating to this boxes object
//
//	//THIS IS NOT GOING TO WORK SINCE THE ENDPOINTS WILL LIKELY HAVE BEEN SWAPPED!!!
//	boxes[boxId].boxId = -1;
//	endpointsX[boxes[boxId].endptId].boxId = -1;
//	endpointsX[boxes[boxId].endptId+1].boxId = -1;
//	endpointsY[boxes[boxId].endptId].boxId = -1;
//	endpointsY[boxes[boxId].endptId+1].boxId = -1;
//
//	for(int i=0;i < MAX_ENCOUNTERS;++i)
//	{
//		if(encounters[i].objIDs[0] == boxId)
//		{
//			this->RemoveEncounter(boxId, encounters[i].objIDs[1]);
//		}
//		else if(encounters[i].objIDs[1] == boxId)
//		{
//			this->RemoveEncounter(boxId, encounters[i].objIDs[0]);
//		}
//	}
//
//	--numBoxes;
//}

//void SweepAndPrune::UpdateBox(int boxId, objType type, double minX, double minY, double maxX, double maxY)
//{
//    // Replace the vals of the box with the inputted boxId
//    // Whenever an object moves it should call this to update its collision box(es)
//    // Each object will know which boxId to pass in since it should store the boxId value
//    // returned when AddBox() is called.
//	boxes[boxId].type = type;
//	boxes[boxId].vals[0][0] = minX;
//	boxes[boxId].vals[0][1] = minY;
//	boxes[boxId].vals[1][0] = maxX;
//	boxes[boxId].vals[1][1] = maxY;
//}