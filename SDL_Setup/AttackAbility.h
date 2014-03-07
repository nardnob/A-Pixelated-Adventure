#ifndef ATTACKABILITY_H
#define ATTACKABILITY_H

#include "Ability.h"

//the default attack ability: a radius-based attack
class AttackAbility : public Ability
{
public:
	AttackAbility();
	void useAbility();
private:
	float
		radius,
		baseDamage;
};

#endif