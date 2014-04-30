#ifndef RADIUSATTACKABILITY_H
#define RADIUSATTACKABILITY_H

#include "Ability.h"

class RadiusAttackAbility : public Ability
{
private:
	float
		radius, //the radius of the radius-based attack
		baseDamage; //the base damage of the attack

	//vector<Entity*>* vector_entities;
public:
	RadiusAttackAbility(Entity* in_ownerEntity, vector<Entity*>* in_vector_entities, float in_radius, float in_baseDamage,
						int in_animationType, int in_animationNum, int in_animationDegradationRate);

	void useAbility();
	int get_animationType() { return this->animationType; }
	int get_animationNum() { return this->animationNum; }
	int get_animationDegradationRate() { return this->animationDegradationRate;	}
};

#endif