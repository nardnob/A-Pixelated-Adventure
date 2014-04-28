#ifndef ANIMATION_H
#define ANIMATION_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "constants.cpp"

class Animation
{
public:
	virtual void display() {}; //for static animations, display at constants (x,y) unique to each object. for following, follow the entity and animate at entity's position. need entity pointer
private:
	int
		degradationRate,
		degradationCount, //the amount degraded since the start of this state. if degradationCount == degradationRate, try to currentState++ and degradationCount reset to 0
		currentState; //will have a max of 3 states per animation, after which the animation should be destroyed. each state affects offset of clip for next frame of animation


	SDL_Rect animationClip;
};

#endif