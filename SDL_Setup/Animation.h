#ifndef ANIMATION_H
#define ANIMATION_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "constants.cpp"

/*
** Animation is an abstract base class for classes: AnimationEntity, AnimationStatic, etc..
*/

class Animation
{
public:
	void display(SDL_Surface* surface_animations, SDL_Surface* surface_screen, double screenOffset_x, double screenOffset_y); //for static animations, display at constants (x,y) unique to each object. for following, follow the entity and animate at entity's position. need entity pointer
	void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
	bool anticipatesDeath() { return commitSuicide; }

	virtual void degrade() {}
protected:
	int
		degradationRate, //wait x frames before degrading to next animation state (think degradationRate / fps = seconds per animation frame)
		degradationCount, //the amount degraded since the start of this state. if degradationCount == degradationRate, try to currentState++ and degradationCount reset to 0
		currentState; //will have a max of 3 states per animation, after which the animation should be destroyed. each state affects offset of clip for next frame of animation

	int
		posX, //static, should not be changed after initialization
		posY; //static, should not be changed after initialization

	bool commitSuicide = false; //should the object be deleted after degrading? is currentState > ANIMATION_MAX_STATES

	SDL_Rect animationClip;
};

#endif