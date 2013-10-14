#include "animobj.h"

AnimObj::AnimObj(SpriteSheet *p_animSheet)
{
	animSheet = p_animSheet;
	playing = false;
	backNforth = false;
	forwards = true;
}

void AnimObj::defineAnim(int index, int numFrames, std::list<aniFrame> p_frames, bool p_backNforth,
						 int p_offset)
{
	aniList newFrames;
	newFrames.index = index;
	newFrames.numFrames = numFrames;
	newFrames.frames = p_frames;
	backNforth = p_backNforth;
	newFrames.offset = p_offset;

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
			if(backNforth)
			{
				if(forwards)
				{
					if(drawFrame->frameID < selAnim->numFrames-1)
						++drawFrame;	//Advance the frame if frame duration is exceeded
					else
					{
						forwards = false;	//Change directions since end frame was reached
						--drawFrame;	//Without this the end frame will render twice
					}
				}
				else
				{
					if(drawFrame->frameID > 0)
						--drawFrame;	//Advance in the opposite direction if duration is exceeded
					else
					{
						forwards = true;	//Change directions since beginning frame was reached
						++drawFrame;	//Without this the beginning frame will render twice
					}
				}
			}
			else
			{
				if(drawFrame->frameID < selAnim->numFrames-1)
					++drawFrame;	//Advance the frame if frame duration is exceeded
				else
					drawFrame = selAnim->frames.begin();	//Loop back to beginning of animation
			}
		}

		animSheet->Draw(drawFrame->frameID + selAnim->offset, dst);
	}
}