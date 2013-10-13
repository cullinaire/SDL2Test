#include "sprite.h"
#include "logger.h"

Sprite::Sprite(const std::string bmpfilename, SDL_Renderer *renderer)
{
	rend = renderer;
	texture = NULL;
	image = SDL_LoadBMP(bmpfilename.c_str());
	if(image != NULL)
	{
		texture = SDL_CreateTextureFromSurface(rend, image);
		//Surface is freed here; texture will be freed in destructor
		SDL_FreeSurface(image);
		if(texture == NULL)
			logSDLError(std::cout, "Create texture from surface failed");
		else
			logSDLError(std::cout, "Loaded texture successfully");
	}
	else
		logSDLError(std::cout, "Loading BMP failed");
}

void Sprite::Draw(SDL_Rect src, SDL_Rect dst)
{
	//SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(rend, texture, &src, &dst);
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(texture);
}

//Constructor for constant cell size sheets
SpriteSheet::SpriteSheet(const std::string bmpfilename, SDL_Renderer *renderer, SDL_Rect cell, int row, int col, int total)
{
	source = new Sprite(bmpfilename.c_str(), renderer);
	sheetinfo = new SheetInfo[total];
	SDL_Rect currentCell = cell;
	currentCell.x = cell.x;
	currentCell.y = cell.y;
	//Populate the sheetinfo array. Param col is number of columns, row is number of rows, cell is the dimensions of each
	//individual cell, and total is the total number of cells to define.
	//Each cell gets a unique id and a unique (hopefully) location on the sprite sheet.
	int rowindex = 0;
	for(int i = 0;i < total;i++)
	{
		if(rowindex == col)	//if end of row is reached, start a new line until total is reached
		{
			currentCell.y += cell.h;
			currentCell.x = 0;
			rowindex = 0;
		}
		sheetinfo[i].id = i;
		sheetinfo[i].cell = currentCell;
		currentCell.x += cell.w;
		rowindex++;
	}
}

//Constructor for arbitrary cell size (and location) sheets
SpriteSheet::SpriteSheet(const std::string bmpfilename, SDL_Renderer *renderer, std::list<SheetInfo> cells, int total)
{
	source = new Sprite(bmpfilename.c_str(), renderer);
	sheetinfo = new SheetInfo[total];
	int i = 0;

	for(std::list<SheetInfo>::iterator itr = cells.begin();itr != cells.end();++itr)
	{
		sheetinfo[i].cell = itr->cell;
		sheetinfo[i].id = i;
		++i;
		if(i > total)
		{
			logSDLError(std::cout, "Number of defined animation cells exceeds provided total!!");
			break;
		}
	}
}

SDL_Rect SpriteSheet::getCellSize()
{
	return sheetinfo->cell;
}

void SpriteSheet::Draw(int id, SDL_Rect dest)
{
	source->Draw(sheetinfo[id].cell, dest);
}

SpriteSheet::~SpriteSheet()
{
	delete source;
	delete sheetinfo;
}