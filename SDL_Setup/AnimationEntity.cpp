#include "AnimationEntity.h"

AnimationEntity::AnimationEntity(int in_posX, int in_posY, int in_animationNum, int in_degradationRate, Entity* in_entity)
{
	this->entity = in_entity;

	this->posX = in_posX;
	this->posY = in_posY;

	this->currentState = 0;
	this->degradationCount = 0;
	this->degradationRate = in_degradationRate; //defaults to FRAMES_PER_SECOND, so one degradation per second

	this->animationClip.w = ANIMATION_CLIP_W;
	this->animationClip.h = ANIMATION_CLIP_H;
	this->animationClip.x = (in_animationNum * ANIMATION_CLIP_W) % ANIMATION_FILE_W;
	this->animationClip.y = in_animationNum * ANIMATION_CLIP_W / ANIMATION_FILE_W * ANIMATION_CLIP_H * (ANIMATION_MAX_STATE + 1);
}

void AnimationEntity::degrade() //if(thatObject.anticaptesDeath()) kill him ... should be checked by caller after caller calls degrade
{
	this->posX = this->entity->posX + ENTITY_CLIP_W / 2 - ANIMATION_CLIP_W / 2;
	this->posY = this->entity->posY + ENTITY_CLIP_H / 2 - ANIMATION_CLIP_H / 2;

	this->degradationCount++;

	if(this->degradationCount >= this->degradationRate)
	{
		this->currentState++;
		this->animationClip.y += ANIMATION_CLIP_H;

		if(this->currentState > ANIMATION_MAX_STATE)
			this->commitSuicide = true;
		this->degradationCount = 0;
	}
}