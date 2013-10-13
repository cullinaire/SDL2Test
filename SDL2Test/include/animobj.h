#ifndef ANIMOBJ_H_
#define ANIMOBJ_H_

//animobj.h - animated object routines
//Provides means for animating objects by iterating through sprite sheets.
//Facilities include start/stop, variable frame rate, switchable sequences, etc.
//Does not do collision detection, input, or anything other than just switching pictures
//around on screen!

#include "sprite.h"
#include <list>
#include "SDL.h"

typedef struct aniFrame
{
	Uint32 duration;	//in ms
	SDL_Rect frameSize;
	int frameID;	//used by SheetInfo to determine which cell to draw
} aniFrame;

typedef struct aniList
{
	int index;
	int numFrames;
	std::list<aniFrame> frames;
} aniList;

class AnimObj
{
public:
	AnimObj(SpriteSheet *p_animSheet);
	void defineAnim(int index, int numFrames, std::list<aniFrame> p_frames);
	void startAnim(int index);
	void playAnim(int x, int y);

private:
	SpriteSheet *animSheet;		//Do not delete - instantiated elsewhere
	bool playing;
	Uint32 elapsed;
	int curAnimIndex;
	std::list<aniList> animations;
	std::list<aniList>::iterator selAnim;
	std::list<aniFrame>::iterator drawFrame;
};

#endif