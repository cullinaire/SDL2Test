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
#include "logger.h"
#include "sprite.h"
#include "gfxtext.h"
#include "menu.h"
#include "inputmap.h"
#include "txtlayer.h"
#include "inputcfg.h"
#include "animobj.h"
#include "player.h"
#include "timekeep.h"

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
	SpriteSheet mm1sheet("../assets/mm1.bmp", rend, "../assets/mm1.def");

	BMPText fontDraw(&arcadeFont);

	TxtLayer mainText(&fontDraw);

	InputMap player1Input;

	InputCfg inputConfig(rend, &mainText, &fontDraw);

	inputConfig.assignPlayerMap(&player1Input);

	AnimObj mm1(&mm1sheet);

	mm1.defineAnim("../assets/mm1ani.def");

	Player player1(&mm1, &inputConfig, 1);

	SDL_Event ev;
	SDL_Scancode lastKey;	//Keeps track of last key pressed

	bool quit = false;
	bool menuactive = false;
	bool waitingForInput = false;	//Used for input mapping
	bool keyPressed[256];	//Used to mitigate key-repeat issues
	for(int i=0;i < 256;++i)
		keyPressed[i] = false;

	inputConfig.registerQuit(&quit);	//So the menu can modify the quit variable...shaky I know

	std::string lastInputMsg;

	SDL_Rect lastInputMsgPos;
	lastInputMsgPos.x = 256;
	lastInputMsgPos.y = 24;

	SDL_Rect advicePos;
	advicePos.x = 8;
	advicePos.y = 8;

	std::string fps;

	SDL_Rect fpsPos;
	fpsPos.x = 24;
	fpsPos.y = 464;

	std::string accum;

	SDL_Rect accumPos;
	accumPos.x = 24;
	accumPos.y = 448;

	std::string time;

	SDL_Rect timePos;
	timePos.x = 24;
	timePos.y = 432;

	double t = 0.0f;
	const double dt = 0.01f;	//fixed timestep for physics updates

	Uint64 currentTime = SDL_GetPerformanceCounter();
	Uint64 sysCounter = 0;	//Is the master timer referenced by game
	Uint64 lastCount = sysCounter;	//For getting the difference vs sysCounter
	double accumulator = 0.0f;
	
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
					player1.assignInput(lastKey);
					waitingForInput = false;
				}
				else if(menuactive)
				{
					player1.configInput(lastKey, &waitingForInput, &menuactive);
				}
				else
				{
					if(keyPressed[lastKey] == false)
					{
						switch(lastKey)
						{
						case SDL_SCANCODE_ESCAPE:
							if(!menuactive)
								menuactive = true;
							else
								menuactive = false;
							break;
						default:
							break;
						}
						player1.processKeyDown(lastKey, keyPressed);
						keyPressed[lastKey] = true;
					}
				}
				lastInputMsg.assign("Last input: ");
				lastInputMsg.append(player1.getInputName(lastKey));
			}
			if(ev.type == SDL_KEYUP)
			{
				lastKey = ev.key.keysym.scancode;	//This might be the fix
				keyPressed[lastKey] = false;
				if(!menuactive)
				{
					player1.processKeyUp(lastKey, keyPressed);
				}
			}
		}

		mainText.ReceiveString("Press ESC to view menu", advicePos);
		mainText.ReceiveString(lastInputMsg, lastInputMsgPos);
		player1.emitInfo(&mainText);

		//UPDATING SECTION////////////////////////////////////////////////////
		UpdateTime(sysCounter, currentTime);	//This must be called only once per frame
		//The following line must be before the two lines after it
		double frameTime = (double)(sysCounter - lastCount)/SDL_GetPerformanceFrequency();
		currentTime = SDL_GetPerformanceCounter();
		lastCount = sysCounter;

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
		//Doing "collision detection" here: (WRONG: DO IT IN FIXED TIMESTEP AREA)

		while(accumulator >= dt)
		{
			accumulator -= dt;
			player1.Collide();
			player1.Integrate(t, dt);
			t += dt;
		}

		time.assign("sysCounter: ");
		time.append(std::to_string(sysCounter));

		mainText.ReceiveString(time, timePos);

		const double alpha = accumulator / dt;

		player1.Interpolate(alpha);

		//DRAWING SECTION/////////////////////////////////////////////////////
		SDL_RenderClear(rend);
		//Draw stuff now
		
		mm1.playAnim();

		if(menuactive)
		{
			inputConfig.showMenu();
			inputConfig.showStatus();
		}
		mainText.OutputFrame(rend);
		mainText.Clear();
		//End draw stuff
		SDL_RenderPresent(rend);
		//SDL_Delay(5);	//Don't peg the CPU
	}

	//Deinitialization
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
