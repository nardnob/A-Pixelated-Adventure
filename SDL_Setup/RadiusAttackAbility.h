#ifndef RADIUSATTACKABILITY_H
#define RADIUSATTACKABILITY_H

#include "Ability.h"

class RadiusAttackAbility : public Ability
{
private:
	float
		radius, //the radius of the radius-based attack
		baseDamage; //the base damage of the attack

	vector<Entity*>* vector_entities;
public:
	RadiusAttackAbility(vector<Entity*>* in_vector_entities, float in_radius, float in_baseDamage);
	void useAbility();
};

#endif