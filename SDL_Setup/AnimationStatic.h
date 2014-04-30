#ifndef ANIMATIONSTATIC_H
#define ANIMATIONSTATIC_H

#include "Animation.h"

/*
** AnimationStatic are animations whose positions are static
*/

class AnimationStatic : public Animation
{
public:
	AnimationStatic(int in_posX, int in_posY, int in_animationNum, int in_degradationRate);

	void degrade();
};

#endif