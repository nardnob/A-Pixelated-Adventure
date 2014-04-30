#include "constants.cpp"
#include "RadiusAttackAbility.h"

#include <math.h>
#include <Windows.h>

using namespace std;

RadiusAttackAbility::RadiusAttackAbility(Entity* in_ownerEntity, vector<Entity*>* in_vector_entities, float in_radius, float in_baseDamage, int in_animationType = 0,
										 int in_animationNum = 0, int in_animationDegradationRate = FRAMES_PER_SECOND)
{
	this->ownerEntity = in_ownerEntity;
	this->vector_entities = in_vector_entities;
	this->radius = in_radius;
	this->baseDamage = in_baseDamage;

	this->animationType = in_animationType;
	this->animationNum = in_animationNum;
	this->animationDegradationRate = in_animationDegradationRate;
}

void RadiusAttackAbility::useAbility()
{
	//OutputDebugString("\n\nBang!!\n\n");
	int
		playerX = vector_entities->at(0)->posX + ENTITY_CLIP_W / 2,
		playerY = vector_entities->at(0)->posY + ENTITY_CLIP_H / 2;

	for(int i = 1; i < vector_entities->size(); i++)
	{
		//had to - ENTITY_CLIP_H / 2 to compensate for the height of the entities.. can't just do point-source distance values
		if(sqrt(pow(vector_entities->at(i)->posX + ENTITY_CLIP_W / 2 - playerX, 2) + pow(vector_entities->at(i)->posY + ENTITY_CLIP_H / 2 - playerY, 2)) - ENTITY_CLIP_H / 2 <= this->radius)
			vector_entities->at(i)->currentStatus.takeLife(this->baseDamage);
	}
}