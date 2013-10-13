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

#include <iostream>
#include <SDL.h>
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

	std::list<SheetInfo> princeCells;
	SheetInfo princeCell;
	SDL_Rect princeSrc;

	std::list<aniFrame> popFrames;
	aniFrame popFrame;

	princeCell.id = popFrame.frameID = 0;
	princeSrc.x = 624;
	princeSrc.y = 758;
	princeSrc.w = 24;
	princeSrc.h = 83;
	princeCell.cell = princeSrc;
	popFrame.frameSize = princeSrc;
	popFrame.duration = 100;
	popFrames.push_back(popFrame);
	princeCells.push_back(princeCell);

	princeCell.id = popFrame.frameID = 1;
	princeSrc.x = 580;
	princeSrc.y = 758;
	princeSrc.w = 28;
	princeSrc.h = 83;
	princeCell.cell = princeSrc;
	popFrame.frameSize = princeSrc;
	popFrame.duration = 100;
	popFrames.push_back(popFrame);
	princeCells.push_back(princeCell);

	princeCell.id = popFrame.frameID = 2;
	princeSrc.x = 525;
	princeSrc.y = 758;
	princeSrc.w = 41;
	princeSrc.h = 83;
	princeCell.cell = princeSrc;
	popFrame.frameSize = princeSrc;
	popFrame.duration = 100;
	popFrames.push_back(popFrame);
	princeCells.push_back(princeCell);

	princeCell.id = popFrame.frameID = 3;
	princeSrc.x = 463;
	princeSrc.y = 758;
	princeSrc.w = 51;
	princeSrc.h = 83;
	princeCell.cell = princeSrc;
	popFrame.frameSize = princeSrc;
	popFrame.duration = 100;
	popFrames.push_back(popFrame);
	princeCells.push_back(princeCell);

	princeCell.id = popFrame.frameID = 4;
	princeSrc.x = 399;
	princeSrc.y = 758;
	princeSrc.w = 54;
	princeSrc.h = 83;
	princeCell.cell = princeSrc;
	popFrame.frameSize = princeSrc;
	popFrame.duration = 100;
	popFrames.push_back(popFrame);
	princeCells.push_back(princeCell);

	princeCell.id = popFrame.frameID = 5;
	princeSrc.x = 331;
	princeSrc.y = 758;
	princeSrc.w = 58;
	princeSrc.h = 83;
	princeCell.cell = princeSrc;
	popFrame.frameSize = princeSrc;
	popFrame.duration = 100;
	popFrames.push_back(popFrame);
	princeCells.push_back(princeCell);

	princeCell.id = popFrame.frameID = 6;
	princeSrc.x = 262;
	princeSrc.y = 758;
	princeSrc.w = 59;
	princeSrc.h = 83;
	princeCell.cell = princeSrc;
	popFrame.frameSize = princeSrc;
	popFrame.duration = 100;
	popFrames.push_back(popFrame);
	princeCells.push_back(princeCell);

	princeCell.id = popFrame.frameID = 7;
	princeSrc.x = 201;
	princeSrc.y = 758;
	princeSrc.w = 54;
	princeSrc.h = 83;
	princeCell.cell = princeSrc;
	popFrame.frameSize = princeSrc;
	popFrame.duration = 100;
	popFrames.push_back(popFrame);
	princeCells.push_back(princeCell);

	princeCell.id = popFrame.frameID = 8;
	princeSrc.x = 145;
	princeSrc.y = 758;
	princeSrc.w = 42;
	princeSrc.h = 83;
	princeCell.cell = princeSrc;
	popFrame.frameSize = princeSrc;
	popFrame.duration = 100;
	popFrames.push_back(popFrame);
	princeCells.push_back(princeCell);

	princeCell.id = popFrame.frameID = 9;
	princeSrc.x = 105;
	princeSrc.y = 758;
	princeSrc.w = 28;
	princeSrc.h = 83;
	princeCell.cell = princeSrc;
	popFrame.frameSize = princeSrc;
	popFrame.duration = 100;
	popFrames.push_back(popFrame);
	princeCells.push_back(princeCell);

	princeCell.id = popFrame.frameID = 10;
	princeSrc.x = 66;
	princeSrc.y = 758;
	princeSrc.w = 23;
	princeSrc.h = 83;
	princeCell.cell = princeSrc;
	popFrame.frameSize = princeSrc;
	popFrame.duration = 100;
	popFrames.push_back(popFrame);
	princeCells.push_back(princeCell);

	princeCell.id = popFrame.frameID = 11;
	princeSrc.x = 23;
	princeSrc.y = 758;
	princeSrc.w = 25;
	princeSrc.h = 83;
	princeCell.cell = princeSrc;
	popFrame.frameSize = princeSrc;
	popFrame.duration = 100;
	popFrames.push_back(popFrame);
	princeCells.push_back(princeCell);

	SpriteSheet arcadeFont("../assets/drbrfont.bmp", rend, fontDim, 1, 95, 95);
	SpriteSheet prince("../assets/pop.bmp", rend, princeCells, 12);

	BMPText fontDraw(&arcadeFont);

	TxtLayer mainText;

	InputMap player1Input;

	InputCfg inputConfig(&player1Input, rend, &mainText, &fontDraw);

	AnimObj pop(&prince);
	pop.defineAnim(0, 12, popFrames);
	pop.startAnim(0);

	SDL_Event ev;
	SDL_Scancode lastKey;

	std::string lastKeyStateMsg;
	lastKeyStateMsg.assign("Press ESC to view menu");

	bool quit = false;
	bool menuactive = false;
	bool waitingForInput = false;

	inputConfig.registerQuit(&quit);

	while(!quit)
	{
		while(SDL_PollEvent(&ev))
		{
			if(ev.type == SDL_QUIT)
				quit = true;
			if(ev.type == SDL_KEYDOWN)
			{
				lastKey = ev.key.keysym.scancode;
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
					}
					lastKeyStateMsg.assign("Last input: ");
					lastKeyStateMsg.append(player1Input.returnInputName(player1Input.returnInput(lastKey)));
				}
				else
				{
					switch(lastKey)
					{
					case SDL_SCANCODE_ESCAPE:
						if(!menuactive)
							menuactive = true;
						else
							menuactive = false;
						break;
					}
				}
			}
		}

		SDL_RenderClear(rend);
		//Draw stuff now
		pop.playAnim(64, 64);
		mainText.Clear();	//What happens if I forget this?!
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