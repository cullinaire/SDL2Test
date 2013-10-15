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

	std::list<SheetInfo> mm1Cells;
	SheetInfo mm1Cell;
	SDL_Rect mm1Src;

	std::list<aniFrame> mm1Frames;
	aniFrame mm1Frame;

	//Run facing left

	mm1Cell.id = 0;
	mm1Src.x = 98;
	mm1Src.y = 94;
	mm1Src.w = 32;
	mm1Src.h = 25;
	mm1Cell.cell = mm1Src;
	mm1Cells.push_back(mm1Cell);

	mm1Cell.id = 1;
	mm1Src.x = 67;
	mm1Src.y = 94;
	mm1Src.w = 32;
	mm1Src.h = 25;
	mm1Cell.cell = mm1Src;
	mm1Cells.push_back(mm1Cell);

	mm1Cell.id = 2;
	mm1Src.x = 35;
	mm1Src.y = 94;
	mm1Src.w = 32;
	mm1Src.h = 25;
	mm1Cell.cell = mm1Src;
	mm1Cells.push_back(mm1Cell);

	//Run facing right

	mm1Cell.id = 3;
	mm1Src.x = 204;
	mm1Src.y = 94;
	mm1Src.w = 32;
	mm1Src.h = 25;
	mm1Cell.cell = mm1Src;
	mm1Cells.push_back(mm1Cell);

	mm1Cell.id = 4;
	mm1Src.x = 236;
	mm1Src.y = 94;
	mm1Src.w = 32;
	mm1Src.h = 25;
	mm1Cell.cell = mm1Src;
	mm1Cells.push_back(mm1Cell);

	mm1Cell.id = 5;
	mm1Src.x = 266;
	mm1Src.y = 94;
	mm1Src.w = 32;
	mm1Src.h = 25;
	mm1Cell.cell = mm1Src;
	mm1Cells.push_back(mm1Cell);

	//Stand and blink facing left

	mm1Cell.id = 6;
	mm1Src.x = 51;
	mm1Src.y = 10;
	mm1Src.w = 23;
	mm1Src.h = 25;
	mm1Cell.cell = mm1Src;
	mm1Cells.push_back(mm1Cell);

	mm1Cell.id = 7;
	mm1Src.x = 76;
	mm1Src.y = 10;
	mm1Src.w = 23;
	mm1Src.h = 25;
	mm1Cell.cell = mm1Src;
	mm1Cells.push_back(mm1Cell);

	//Stand and blink facing right

	mm1Cell.id = 8;
	mm1Src.x = 235;
	mm1Src.y = 10;
	mm1Src.w = 23;
	mm1Src.h = 25;
	mm1Cell.cell = mm1Src;
	mm1Cells.push_back(mm1Cell);

	mm1Cell.id = 9;
	mm1Src.x = 260;
	mm1Src.y = 10;
	mm1Src.w = 23;
	mm1Src.h = 25;
	mm1Cell.cell = mm1Src;
	mm1Cells.push_back(mm1Cell);

	SpriteSheet arcadeFont("../assets/drbrfont.bmp", rend, fontDim, 1, 95, 95);
	SpriteSheet mm1sheet("../assets/mm1.bmp", rend, mm1Cells, mm1Cells.size());

	BMPText fontDraw(&arcadeFont);

	TxtLayer mainText;

	InputMap player1Input;

	InputCfg inputConfig(&player1Input, rend, &mainText, &fontDraw);

	AnimObj mm1(&mm1sheet);

	//Run facing left, anim id = 0

	mm1Frame.frameID = 0;
	mm1Frame.duration = 200;
	mm1Frames.push_back(mm1Frame);

	mm1Frame.frameID = 1;
	mm1Frame.duration = 200;
	mm1Frames.push_back(mm1Frame);

	mm1Frame.frameID = 2;
	mm1Frame.duration = 200;
	mm1Frames.push_back(mm1Frame);

	mm1.defineAnim(0, 3, mm1Frames, true, 0);
	
	mm1Frames.clear();

	//Run facing right, anim id = 1

	mm1Frame.frameID = 0;
	mm1Frame.duration = 200;
	mm1Frames.push_back(mm1Frame);

	mm1Frame.frameID = 1;
	mm1Frame.duration = 200;
	mm1Frames.push_back(mm1Frame);

	mm1Frame.frameID = 2;
	mm1Frame.duration = 200;
	mm1Frames.push_back(mm1Frame);

	mm1.defineAnim(1, 3, mm1Frames, true, 3);
	
	mm1Frames.clear();

	//Stand facing left, anim id = 2

	mm1Frame.frameID = 0;
	mm1Frame.duration = 100;
	mm1Frames.push_back(mm1Frame);

	mm1Frame.frameID = 1;
	mm1Frame.duration = 1500;
	mm1Frames.push_back(mm1Frame);

	mm1.defineAnim(2, 2, mm1Frames, true, 6);
	
	mm1Frames.clear();

	//Stand facing right, anim id = 3

	mm1Frame.frameID = 0;
	mm1Frame.duration = 1500;
	mm1Frames.push_back(mm1Frame);

	mm1Frame.frameID = 1;
	mm1Frame.duration = 100;
	mm1Frames.push_back(mm1Frame);

	mm1.defineAnim(3, 2, mm1Frames, true, 8);
	
	mm1Frames.clear();

	//Done defining animations

	std::string lastAnimMsg;
	lastAnimMsg.assign("Standing facing Left playing");

	mm1.startAnim(2);

	SDL_Event ev;
	SDL_Scancode lastKey;	//Keeps track of last key pressed

	std::string lastKeyStateMsg;
	lastKeyStateMsg.assign("Press ESC to view menu");
	std::string leftKeyState;
	leftKeyState.assign("left Key not pressed");
	std::string rightKeyState;
	rightKeyState.assign("right Key not pressed");

	bool quit = false;
	bool menuactive = false;
	bool waitingForInput = false;	//Used for input mapping
	bool keyPressed[256];	//Used to mitigate key-repeat issues
	for(int i=0;i < 256;++i)
		keyPressed[i] = false;

	inputConfig.registerQuit(&quit);	//So the menu can modify the quit variable...shaky I know

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
					inputConfig.assignInput(lastKey);
					waitingForInput = false;
				}
				else if(menuactive)
				{
					switch(lastKey)
					{
					case SDL_SCANCODE_DOWN:
						inputConfig.menuDown();
						break;
					case SDL_SCANCODE_UP:
						inputConfig.menuUp();
						break;
					case SDL_SCANCODE_RETURN:
						inputConfig.menuSelect();
						waitingForInput = true;
						break;
					case SDL_SCANCODE_ESCAPE:
						if(!menuactive)
							menuactive = true;
						else
							menuactive = false;
						break;
					default:
						break;
					}
					lastKeyStateMsg.assign("Last input: ");
					lastKeyStateMsg.append(player1Input.returnInputName(player1Input.returnInput(lastKey)));
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
						switch(player1Input.returnInput(lastKey))
						{
							//Thinking about implementing a Spelunky style approach to simultaneous
							//keydowns for left and right move inputs. That is, if one is pressed
							//while the other was already being held down, then the response is for
							//the player to stop in his tracks, without turning around.
							//If a key is released, the player begins moving in the direction of the
							//key that is still held down.
						case MOVE_LEFT:
							if(keyPressed[player1Input.returnScancode(MOVE_RIGHT)] == true)
							{
								mm1.startAnim(3);
								lastAnimMsg.assign("Standing facing right playing");
							}
							else
							{
								mm1.startAnim(0);
								lastAnimMsg.assign("Running Left playing");
							}
								leftKeyState.assign("left Key pressed");
							break;
						case MOVE_RIGHT:
							if(keyPressed[player1Input.returnScancode(MOVE_LEFT)] == true)
							{
								mm1.startAnim(2);
								lastAnimMsg.assign("Standing facing left playing");
							}
							else
							{
								mm1.startAnim(1);
								lastAnimMsg.assign("Running Right playing");
							}
								rightKeyState.assign("right Key pressed");
							break;
						default:
							break;
						}
						keyPressed[lastKey] = true;
					}
				}
			}
			if(ev.type == SDL_KEYUP)
			{
				lastKey = ev.key.keysym.scancode;	//This might be the fix
				keyPressed[lastKey] = false;
				if(!menuactive)
				{
					switch(player1Input.returnInput(lastKey))
					{
					case MOVE_LEFT:
						if(keyPressed[player1Input.returnScancode(MOVE_RIGHT)] == true)
						{
							mm1.startAnim(1);
							lastAnimMsg.assign("Running Right playing");
						}
						else
						{
							mm1.startAnim(2);
							lastAnimMsg.assign("Standing facing left playing");
						}
						leftKeyState.assign("left Key not pressed");
						break;
					case MOVE_RIGHT:
						if(keyPressed[player1Input.returnScancode(MOVE_LEFT)] == true)
						{
							mm1.startAnim(0);
							lastAnimMsg.assign("Running Left playing");
						}
						else
						{
							mm1.startAnim(3);
							lastAnimMsg.assign("Standing facing right playing");
						}
						rightKeyState.assign("right Key not pressed");
						break;
					default:
						break;
					}
				}
			}
		}

		SDL_RenderClear(rend);
		//Draw stuff now
		mm1.playAnim(256, 64);
		mainText.Clear();	//What happens if I forget this?!
		mainText.ReceiveString(leftKeyState, 256, 128, fontDim, &fontDraw);
		mainText.ReceiveString(rightKeyState, 256, 144, fontDim, &fontDraw);
		mainText.ReceiveString(lastAnimMsg, 256, 16, fontDim, &fontDraw);
		mainText.ReceiveString(lastKeyStateMsg, 256, 32, fontDim, &fontDraw);
		if(menuactive)
		{
			inputConfig.showMenu();
			inputConfig.showStatus();
		}
		mainText.OutputFrame(rend);
		//End draw stuff
		SDL_RenderPresent(rend);
		SDL_Delay(5);	//Don't peg the CPU
	}

	//Deinitialization
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
