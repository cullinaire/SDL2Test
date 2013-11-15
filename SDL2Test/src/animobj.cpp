#include "animobj.h"

AnimObj::AnimObj(SpriteSheet *p_animSheet)
{
	animSheet = p_animSheet;
	playing = false;
	forwards = true;
	isbNf = false;
	dst.x = 0;
	dst.y = 0;
}

void AnimObj::defineAnim(std::string anifilename)
{
	aniList newFrames;
	aniFrame newFrame;

	std::ifstream anideffile;
	anideffile.open(anifilename, std::ios::in);
	std::string::iterator itr;
	int frameid = 0;
	int animid = 0;
	int offset = 0;

	if(anideffile.is_open())
	{
		std::string line, number;
		number.clear();
		while(std::getline(anideffile, line))
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
				if(*itr == '[')	//denotes beginning of frame
				{
					++itr;
					for(;itr != line.end();++itr)	//begin reading values
					{
						if(*itr == ']')	//end of block, write value and exit loop (line)
						{
							newFrame.duration = std::stoi(number, NULL, 10);
							newFrame.frameID = frameid;
							newFrames.frames.push_back(newFrame);
							++frameid;
							++offset;
							number.clear();
							break;
						}
						number.push_back(*itr);
					}
				}
				else if(*itr == '{')	//denotes anim definition
				{
					++itr;
					for(;itr != line.end();++itr)
					{
						if(*itr == '}')	//end of block
						{
							if(number == "true")
							{
								newFrames.backNforth = true;
								newFrames.index = animid;
								newFrames.numFrames = frameid;
								newFrames.offset = offset - newFrames.numFrames;
								frameid = 0;
								++animid;
							}
							else if(number == "false")
							{
								newFrames.backNforth = false;
								newFrames.index = animid;
								newFrames.numFrames = frameid;
								newFrames.offset = offset - newFrames.numFrames;
								frameid = 0;
								++animid;
							}

							number.clear();
							break;
						}
						number.push_back(*itr);
					}
					animations.push_back(newFrames);
					newFrames.frames.clear();	//Don't forget this!!
				}
			}
		}
	}
}

void AnimObj::startAnim(int index)
{
	playing = true;
	if(curAnimIndex != index)	//Prevent repeat calls from interrupting animation
	{
		curAnimIndex = index;
		elapsed = SDL_GetTicks();
		for(selAnim = animations.begin();selAnim != animations.end();++selAnim)
		{
			if(selAnim->index == index)	//Found the animation to play
			{
				drawFrame = selAnim->frames.begin();	//Set the iterator for later (actually this doesn't seem to work)
				isbNf = selAnim->backNforth;
				break;	//also need to preserve selAnim iterator as well (also doesn't work across method calls)
			}
		}
	}
}

//This will always be called in the main loop, but will only actally render if an animation is active
void AnimObj::playAnim()
{
	if(playing)
	{
		if(SDL_GetTicks() - elapsed >= drawFrame->duration)
		{
			elapsed = SDL_GetTicks();
			//I can't use iterators here because they just simply will not work
			//Which is odd, since their values have not changed since startAnim was called.
			//Oh well I guess.
			if(isbNf)
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

		animSheet->getCellSize(drawFrame->frameID, dst);
		animSheet->Draw(drawFrame->frameID + selAnim->offset, dst);
	}
}

void AnimObj::updateLoc(int x, int y)
{
	dst.x = x;
	dst.y = y;
}