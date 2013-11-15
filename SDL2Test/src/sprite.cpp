#include "sprite.h"
#include "logger.h"

Sprite::Sprite(const std::string bmpfilename, SDL_Renderer *renderer)
{
	rend = renderer;
	texture = NULL;
	image = SDL_LoadBMP(bmpfilename.c_str());
	if(image != NULL)
	{
		//Set transparent pixel as pixel at (0,0)
		if(image->format->palette)
		{
			SDL_SetColorKey(image, 1, *(Uint8 *) image->pixels);
		}
		else
		{
			switch(image->format->BitsPerPixel)
			{
			case 15:
				SDL_SetColorKey(image, 1, (*(Uint16 *) image->pixels) & 0x00007FFF);
				break;
			case 16:
				SDL_SetColorKey(image, 1, *(Uint16 *) image->pixels);
				break;
			case 24:
				SDL_SetColorKey(image, 1, (*(Uint32 *) image->pixels) & 0x00FFFFFF);
				break;
			case 32:
				SDL_SetColorKey(image, 1, *(Uint32 *) image->pixels);
				break;
			}
		}

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

//Constructor for arbitrary cell size (and location) sheets. Reads info from a text file.
SpriteSheet::SpriteSheet(const std::string bmpfilename, SDL_Renderer *renderer, const std::string sprdeffilename)
{
	std::ifstream sprdeffile;
	sprdeffile.open(sprdeffilename, std::ios::in);
	std::list<SheetInfo> cells;
	std::string::iterator itr;
	SheetInfo cell;
	int id = 0;
	int cellElement = 0;	//chooses which element of sheetinfo to fill out

	if(sprdeffile.is_open())
	{
		std::string line, number;
		number.clear();
		while(std::getline(sprdeffile, line))
		{
			//if the line begins with #, it's a comment, do nothing
			//Also skip blank lines
			if(line[0] != '#' && line.size() > 0)
			{
				for(itr = line.begin();itr != line.end();++itr)
				{
					if(*itr != ' ' && *itr != '\t')	//skip initial whitespace (incl. tabs)
						break;
				}
				if(*itr == '[')	//denotes beginning of frame block
				{
					cellElement = 0;
					++itr;
					for(;itr != line.end();++itr)	//begin reading values
					{
						if(*itr != ' ')	//skip whitespace between values
						{
							if(*itr == ',')	//save current number to appropriate value and advance
							{
								switch(cellElement)
								{
								case 0:
									cell.cell.x = std::stoi(number, NULL, 10);
									break;
								case 1:
									cell.cell.y = std::stoi(number, NULL, 10);
									break;
								case 2:
									cell.cell.w = std::stoi(number, NULL, 10);
									break;
								}
								++cellElement;
								number.clear();
							}
							else if(*itr == ']')	//end of block, write last value and exit
							{
								cell.cell.h = std::stoi(number, NULL, 10);
								number.clear();
								break;
							}

							if(*itr != ' ' && *itr != ',')
								number.push_back(*itr);	//append() does not work with chars, so using push_back()
						}
					}
					cell.id = id;
					cells.push_back(cell);
					++id;
				}	//end of one block
			}
		}
	}
	else
	{
		std::cout << "Could not open sprite definition file " << sprdeffilename << "." << std::endl;
	}

	sprdeffile.close();

	source = new Sprite(bmpfilename.c_str(), renderer);
	sheetinfo = new SheetInfo[cells.size()];

	int i = 0;
	for(std::list<SheetInfo>::iterator sitr = cells.begin();sitr != cells.end();++sitr)
	{
		sheetinfo[i].cell = sitr->cell;
		sheetinfo[i].id = sitr->id;
		++i;
	}
}

SDL_Rect SpriteSheet::getCellSize(int idx)
{
	return sheetinfo[idx].cell;
}

void SpriteSheet::getCellSize(int idx, SDL_Rect &dst)
{
	dst.w = sheetinfo[idx].cell.w;
	dst.h = sheetinfo[idx].cell.h;
}

void SpriteSheet::Draw(int id, SDL_Rect dest)
{
	dest.w = sheetinfo[id].cell.w;
	dest.h = sheetinfo[id].cell.h;

	source->Draw(sheetinfo[id].cell, dest);
}

SpriteSheet::~SpriteSheet()
{
	delete source;
	delete sheetinfo;
}