#include "collision.h"

bool Endpoint::operator<(const int val)
{
	return this->val < val;
}

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

void SweepAndPrune::Update()
{
	//Do X axis stuff first

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
				++numEncounters;
				break;	//Forgot this again...
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
					--numEncounters;
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
					--numEncounters;
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
		if(boxes[i].boxId == -1)	//Find the first empty cell
		{
			boxes[i].boxId = i;
			boxes[i].type = type;
			boxes[i].vals[0][0] = minX;
			boxes[i].vals[0][1] = minY;
			boxes[i].vals[1][0] = maxX;
			boxes[i].vals[1][1] = maxY;

			// add endpoints
			endpointsX[2*i].type = 0;		//xmin
			endpointsX[2*i].boxId = i;
			endpointsX[2*i].val = minX;

			endpointsX[2*i+1].type = 1;		//xmax
			endpointsX[2*i+1].boxId = i;
			endpointsX[2*i+1].val = maxX;

			endpointsY[2*i].type = 0;		//ymin
			endpointsY[2*i].boxId = i;
			endpointsY[2*i].val = minY;

			endpointsY[2*i+1].type = 1;		//ymax
			endpointsY[2*i+1].boxId = i;
			endpointsY[2*i+1].val = maxY;

			boxID = i;
			boxes[i].endptId = 2*i;
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

	//THIS IS NOT GOING TO WORK SINCE THE ENDPOINTS WILL LIKELY HAVE BEEN SWAPPED!!!
	boxes[boxId].boxId = -1;
	endpointsX[boxes[boxId].endptId].boxId = -1;
	endpointsX[boxes[boxId].endptId+1].boxId = -1;
	endpointsY[boxes[boxId].endptId].boxId = -1;
	endpointsY[boxes[boxId].endptId+1].boxId = -1;

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