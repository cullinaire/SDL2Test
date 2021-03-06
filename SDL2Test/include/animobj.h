#ifndef ANIMOBJ_H_
#define ANIMOBJ_H_

//animobj.h - animated object routines
//Provides means for animating objects by iterating through sprite sheets.
//Facilities include start/stop, variable frame rate, switchable sequences, etc.
//Does not do collision detection, input, or anything other than just switching pictures
//around on screen!

#ifdef __linux
	#include "SDL2/SDL.h"
#elif _WIN32
	#include "SDL.h"
#endif
#include "sprite.h"
#include <iostream>
#include <fstream>
#include <list>

typedef struct aniFrame
{
	double duration;	//in ms
	int frameID;	//used by SheetInfo to determine which cell to draw
} aniFrame;

typedef struct aniList
{
	int index;
	int numFrames;
	int offset;		//FrameID always starts from 0, so this is needed to map
	bool backNforth;	//instead of traditional looping, bounce backwards when end is reached
	std::list<aniFrame> frames;
} aniList;

class AnimObj
{
public:
	AnimObj();
	AnimObj(SpriteSheet *p_animSheet);
	void loadSheet(SpriteSheet *p_animSheet);
	void defineAnim(std::string anifilename);
	void startAnim();
	void stopAnim();
	void playAnim(int index, double t);
	void outputCurFrame();
	void changeSpeed(double mult);
	void updateLoc(int x, int y);

private:
	SpriteSheet *animSheet;		//Do not delete - instantiated elsewhere
	bool playing;
	bool forwards;		//direction animation is playing in - only used if backNforth is true
	bool isbNf;		//mode is back and forth
	double speedFactor;	//Multiplied by frame duration to vary playback speed
	double elapsed;
	int curAnimIndex;
	int curAnimFrameID;
	SDL_Rect dst;
	std::list<aniList> animations;
	std::list<aniList>::iterator selAnim;
	std::list<aniFrame>::iterator drawFrame;
};

#endif
