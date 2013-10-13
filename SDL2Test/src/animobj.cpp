#include "animobj.h"

AnimObj::AnimObj(SpriteSheet *p_animSheet)
{
	animSheet = p_animSheet;
	playing = false;
}

void AnimObj::defineAnim(int index, int numFrames, std::list<aniFrame> p_frames)
{
	aniList newFrames;
	newFrames.index = index;
	newFrames.numFrames = numFrames;
	newFrames.frames = p_frames;

	animations.push_back(newFrames);
}

void AnimObj::startAnim(int index)
{
	playing = true;
	curAnimIndex = index;
	elapsed = SDL_GetTicks();
	for(selAnim = animations.begin();selAnim != animations.end();++selAnim)
	{
		if(selAnim->index == index)	//Found the animation to play
		{
			drawFrame = selAnim->frames.begin();	//Set the iterator for later (actually this doesn't seem to work)
			break;	//also need to preserve selAnim iterator as well (also doesn't work across method calls)
		}
	}
}

//This will always be called in the main loop, but will only actally render if an animation is active
void AnimObj::playAnim(int x, int y)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;

	if(playing)
	{
		if(SDL_GetTicks() - elapsed >= drawFrame->duration)
		{
			elapsed = SDL_GetTicks();
			//I can't use iterators here because they just simply will not work
			//Which is odd, since their values have not changed since startAnim was called.
			//Oh well I guess.
			if(drawFrame->frameID < selAnim->numFrames-1)
				++drawFrame;	//Advance the frame if frame duration is exceeded
			else
				drawFrame = selAnim->frames.begin();	//Loop back to beginning of animation
		}
		dst.w = drawFrame->frameSize.w;
		dst.h = drawFrame->frameSize.h;

		animSheet->Draw(drawFrame->frameID, dst);
	}
}