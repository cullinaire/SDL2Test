#include <iostream>
#include <SDL.h>
#include "logger.h"
#include "sprite.h"
#include "gfxtext.h"
#include "menu.h"
#include <string>
#include <list>

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
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

	SDL_Rect dest;
	dest.x = 16;
	dest.y = 16;
	dest.w = 8;	//Do not forget to set the destination width and height as well!
	dest.h = 8;	//Failing to do so will result in mysterious results. Rarely amusing.
	SDL_Rect fontDim;
	fontDim.x = 0;
	fontDim.y = 0;	//color of text: 0 = orange, 8 = gold, 16 = green
	fontDim.w = 8;
	fontDim.h = 8;

	SpriteSheet *arcadeFont = new SpriteSheet("../assets/drbrfont.bmp", rend, fontDim, 1, 95, 95);
	fontDim.y = 8;
	SpriteSheet *goldFont = new SpriteSheet("../assets/drbrfont.bmp", rend, fontDim, 1, 95, 95);
	fontDim.y = 16;
	SpriteSheet *greenFont = new SpriteSheet("../assets/drbrfont.bmp", rend, fontDim, 1, 95, 95);

	BMPText fontDraw = BMPText(arcadeFont);

	std::list<std::string> testMenuItems;

	testMenuItems.push_back("one");
	testMenuItems.push_back("two");
	testMenuItems.push_back("tree");

	Menu testMenu = Menu(&fontDraw, &testMenuItems);

	SDL_Event ev;
	bool quit = false;

	while(!quit)
	{
		while(SDL_PollEvent(&ev))
		{
			if(ev.type == SDL_QUIT)
				quit = true;
			if(ev.type == SDL_KEYDOWN)
			{
				if(ev.key.keysym.scancode == SDL_SCANCODE_UP)
					testMenu.MoveCursor(false);
				else if(ev.key.keysym.scancode == SDL_SCANCODE_DOWN)
					testMenu.MoveCursor(true);
				else if(ev.key.keysym.scancode == SDL_SCANCODE_RETURN)
				{
					switch(testMenu.ExecuteItem())
					{
					case 0:
						fontDraw.ChangeFont(arcadeFont);
						break;
					case 1:
						fontDraw.ChangeFont(goldFont);
						break;
					case 2:
						fontDraw.ChangeFont(greenFont);
						break;
					default:
						fontDraw.ChangeFont(arcadeFont);
						break;
					}
				}
				else
					quit = true;
			}
		}

		SDL_RenderClear(rend);
		//Draw stuff now
		testMenu.DisplayMenu(8, 8);
		SDL_RenderPresent(rend);
		SDL_Delay(5);
	}

		/*fontDraw.PrintText("i can sleep now.", 8, 8, fontDim);
		SDL_RenderPresent(rend);
		SDL_Delay(1000);
		fontDraw.PrintText("the locals called him \"beast\"!0123456789", 8, 16, fontDim);
		SDL_RenderPresent(rend);
		SDL_Delay(1000);
		fontDraw.PrintText("t('.')t #rofl::[][]{}8===d~~abcdefghijklmnopqrstuvwxyz", 8, 24, fontDim);
		SDL_RenderPresent(rend);
		SDL_Delay(1000);
		fontDraw.PrintText("what gives us this \'challenge\'?", 8, 32, fontDim);
		SDL_RenderPresent(rend);
		SDL_Delay(1000);
		fontDraw.PrintText("\\o/ victory (to the japanese since the backslash is a yen sign now)", 8, 40, fontDim);
		SDL_RenderPresent(rend);
		SDL_Delay(5000);*/

	//Deinitialization
	delete arcadeFont;
	delete goldFont;
	delete greenFont;
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}