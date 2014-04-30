#ifndef ANIMATIONENTITY_H
#define ANIMATIONENTITY_H

#include "Animation.h"
#include "Entity.h"

/*
** AnimationEntity are animations which follow an entity, updating position through each degrade via a stored Entity*
*/


class AnimationEntity : public Animation
{
public:
	Entity* ownerEntity;

	AnimationEntity(int in_posX, int in_posY, int in_animationNum, int in_degradationRate, Entity* in_ownerEntity);

	Entity* get_ownerEntity() { return this->ownerEntity; }

	void degrade(); //if(thatObject.anticaptesDeath()) kill him ... should be checked by caller after caller calls degrade
};

#endif