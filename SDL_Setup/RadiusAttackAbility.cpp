#include "constants.cpp"
#include "RadiusAttackAbility.h"

#include <math.h>
#include <Windows.h>

using namespace std;

RadiusAttackAbility::RadiusAttackAbility(vector<Entity*>* in_vector_entities, float in_radius, float in_baseDamage)
{
	this->vector_entities = in_vector_entities;
	this->radius = in_radius;
	this->baseDamage = in_baseDamage;
}

void RadiusAttackAbility::useAbility()
{
	//OutputDebugString("\n\nBang!!\n\n");
	int
		playerX = vector_entities->at(0)->posX + ENTITY_CLIP_W / 2,
		playerY = vector_entities->at(0)->posY + ENTITY_CLIP_H / 2;

	for(int i = 1; i < vector_entities->size(); i++)
	{
		if(sqrt(pow(vector_entities->at(i)->posX + ENTITY_CLIP_W / 2 - playerX, 2) + pow(vector_entities->at(i)->posY + ENTITY_CLIP_H / 2 - playerY, 2)) <= this->radius)
			vector_entities->at(i)->currentStatus.takeLife(this->baseDamage);
	}
}