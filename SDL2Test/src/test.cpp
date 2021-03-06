//SDL2Test - testing SDL 2.0 as well as figuring out how to use it in a real program
//Currently working with bitmap text output as well as a rudimentary input configuration
//scheme.
//
//NOTES:
//The text output is structured as follows:
//The lowest level is a sprite class that loads an image file and is able to output it to
//screen. This is used by a spritesheet class that can output just a specific part of the image.
//This then allows the bitmap text class to map the appropriate parts of a font spritesheet to
//character types (currently using ASCII indices).
//The menu class uses the bitmap text class to draw its text. It also accepts keyboard input to
//facilitate navigation.
//Currently I am working on a text layer class that will collect all text generated for each frame
//and output it simultaneously. This will make it easier to decouple text output from other
//functions.
//As I am working on input next, this will be critical. Decoupling allows text to continually
//update while other items are being modified. In the end, there should not be any one system that
//holds the main loop up. Each system should make its data modification and rely on the next cycle
//to continue processing.
//Input processing must be the same. For example, if the inputcfg class is waiting for the player
//to enter a key, the main loop must not spin around that point. Set a variable noting that the
//function is waiting, but continue on with the rest of the loop. If the player finally enters the
//key, then do whatever with it.

#ifdef __linux
	#include "SDL2/SDL.h"
#elif _WIN32
	#include "SDL.h"
#endif
#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>
#include <chrono>
#include "logger.h"
#include "sprite.h"
#include "gfxtext.h"
#include "menu.h"
#include "txtlayer.h"
#include "inputcfg.h"
#include "animobj.h"
#include "player.h"
#include "timekeep.h"
#include "collision.h"

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER) != 0)
	{
		logSDLError(std::cout, "SDL_Init Error");
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Happy", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if(window == NULL)
	{
		logSDLError(std::cout, "Creation of window failed");
		return 2;
	}

	SDL_Renderer *rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if(rend == NULL)
	{
		logSDLError(std::cout, "Creation of renderer failed");
		return 3;
	}

	//END OF INIT//////////////////////////////////////////////////////////////

	SDL_Rect fontDim;
	fontDim.x = 0;
	fontDim.y = 0;	//color of text: 0 = orange, 8 = gold, 16 = green
	fontDim.w = 8;
	fontDim.h = 8;

	SpriteSheet arcadeFont("../assets/drbrfont.bmp", rend, fontDim, 1, 95, 95);

	SpriteSheet mensheet("../assets/men.bmp", rend, "../assets/hatman.def");

	SpriteSheet itemsheet("../assets/men.bmp", rend, "../assets/pickups.def");

	BMPText fontDraw(&arcadeFont);	//Default system font for debug msgs

	TxtLayer mainText(&fontDraw);	//Txtlayer which will carry all printed text for each frame

	InputCfg inputConfig(rend, &mainText, &fontDraw);	//Helper to configure input for each player

	SweepAndPrune collider;

	PlayerGroup players;

	SDL_Event ev;
	SDL_Scancode lastKey;	//Keeps track of last key pressed

	bool quit = false;
	bool drawAABBs = false;
	bool playerSelectMenu = false;
	bool inputMenu = false;
	bool waitingForInput = false;	//Used for input mapping
	bool keyPressed[256];	//Used to mitigate key-repeat issues
	int activePlayerId = 0;
	for(int i=0;i < 256;++i)
		keyPressed[i] = false;

	/*DEBUGGING INFO STUFF*/
	std::string coll;

	SDL_Rect collPos;
	collPos.x = 0;
	collPos.y = 400;

	std::string fps;

	SDL_Rect fpsPos;
	fpsPos.x = 0;
	fpsPos.y = 416;

	std::string accum;

	SDL_Rect accumPos;
	accumPos.x = 0;
	accumPos.y = 432;

	std::string time;

	SDL_Rect timePos;
	timePos.x = 0;
	timePos.y = 448;

	std::string moveforce;

	SDL_Rect velPos;
	velPos.x = 0;
	velPos.y = 464;
	/*END DEBUGGING INFO STUFF*/

	double t = 0.0f;
	const double dt = 0.01f;	//fixed timestep for physics updates

	Uint64 currentTime = SDL_GetPerformanceCounter();
	Uint64 sysCounter = 0;	//Is the master timer referenced by game
	Uint64 lastCount = sysCounter;	//For getting the difference vs sysCounter
	double accumulator = 0.0f;
	
	//Randomizer Stuff
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distrib(0, MAXPLAYERS-1);

	while(!quit)
	{		
		while(SDL_PollEvent(&ev))
		{
			if(ev.type == SDL_QUIT)
				quit = true;
			if(ev.type == SDL_KEYDOWN)
			{
				lastKey = ev.key.keysym.scancode;	//Also need to do this for Key Up event
				if(waitingForInput)
				{
					//do the key assignment here
					//players[activePlayerId].assignInput(lastKey);
					waitingForInput = false;
				}
				else if(playerSelectMenu)
				{
					switch(lastKey)
					{
					case SDL_SCANCODE_UP:
						//playerSelect.MoveCursor(false);
						break;
					case SDL_SCANCODE_DOWN:
						//playerSelect.MoveCursor(true);
						break;
					case SDL_SCANCODE_RETURN:
						inputMenu = true;
						playerSelectMenu = false;
						//activePlayerId = playerSelect.ExecuteItem();
						lastKey = SDL_SCANCODE_F24;	//Just a placeholder to prevent inputmenu from processing the return
						//configinput will reset inputMenu to false upon exit
						//players[activePlayerId].configInput(lastKey, &waitingForInput, &inputMenu);
						break;
					case SDL_SCANCODE_ESCAPE:
						playerSelectMenu = false;
						break;
					default:
						break;
					}
				}
				else if(inputMenu)
				{
					//players[activePlayerId].configInput(lastKey, &waitingForInput, &inputMenu);
					if(!inputMenu)
						playerSelectMenu = true;
				}
				else
				{
					if(keyPressed[lastKey] == false)
					{
						int newId = 0;
						switch(lastKey)
						{
						case SDL_SCANCODE_ESCAPE:
							if(!playerSelectMenu)
								playerSelectMenu = true;
							else
								playerSelectMenu = false;
							break;
						case SDL_SCANCODE_F1:	//To toggle visible AABBs
							if(!drawAABBs)
								drawAABBs = true;
							else
								drawAABBs = false;
							break;
						case SDL_SCANCODE_F2:	//Add players
							newId = distrib(generator);
							while(players.PlayerExists(newId))
							{
								/*seed = std::chrono::system_clock::now().time_since_epoch().count();
								generator.seed(seed);*/
								if(players.is_Full())
									break;
								newId = distrib(generator);
							}

							players.Add(newId, collider, mensheet, inputConfig);

							if(newId == 1)
							{
								players.AssignInput(newId, "../assets/player1bind.def");
							}
							break;
						case SDL_SCANCODE_F3:	//To remove dummy players starting from id 7
							newId = distrib(generator);
							while(!players.PlayerExists(newId))
							{
								if(players.is_Empty())
									break;
								newId = distrib(generator);
							}

							players.Remove(newId, collider);
							break;
						case SDL_SCANCODE_F4:	//Testing impulse function
							players.RandomImpulse();
							break;
						default:
							break;
						}

						keyPressed[lastKey] = true;
						players.ProcessInput(true, lastKey, keyPressed);
					}
				}
			}
			if(ev.type == SDL_KEYUP)
			{
				lastKey = ev.key.keysym.scancode;	//This might be the fix
				keyPressed[lastKey] = false;
				players.ProcessInput(false, lastKey, keyPressed);
			}
		}

		//UPDATING SECTION////////////////////////////////////////////////////
		UpdateTime(sysCounter, currentTime);	//This must be called only once per frame
		//The following line must be before the two lines after it
		double frameTime = (double)(sysCounter - lastCount)/SDL_GetPerformanceFrequency();	//1st
		currentTime = SDL_GetPerformanceCounter();											//2nd
		lastCount = sysCounter;																//3rd

		if(frameTime > 0.25f)
			frameTime = 0.25f;	//Max frame time to avoid sprial of death

		fps.assign("Frametime: ");
		fps.append(std::to_string(frameTime));

		mainText.ReceiveString(fps, fpsPos);	//report fps

		accumulator += frameTime;

		accum.assign("Accumulator: ");
		accum.append(std::to_string(accumulator));

		mainText.ReceiveString(accum, accumPos);

		//Now start doing expensive stuff
		while(accumulator >= dt)	//The fixed timestep area is in this while loop
		{
			accumulator -= dt;
			players.Update(t, dt, collider, moveforce);

			mainText.ReceiveString(coll, collPos);
			mainText.ReceiveString(moveforce, velPos);

			t += dt;
		}	//END FIXED TIMESTEP SECTION

		time.assign("sysCounter: ");
		time.append(std::to_string(sysCounter));

		mainText.ReceiveString(time, timePos);

		const double alpha = accumulator / dt;

		//DRAWING SECTION/////////////////////////////////////////////////////
		//Prepare screen
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);	//Set back to black if AABB set to red
		SDL_RenderClear(rend);

		//Draw stuff now
		players.Render(alpha, t);

		if(drawAABBs)
			collider.drawBoundingBoxes(rend);

		mainText.OutputFrame(rend);	//Draw text last
		mainText.Clear();

		//Output results to screen
		SDL_RenderPresent(rend);
	}	//END MAIN LOOP

	//Deinitialization
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
