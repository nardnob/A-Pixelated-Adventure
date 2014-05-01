#ifndef ABILITY_H
#define ABILITY_H

#include <vector>
using namespace std;

#include "Entity.h"

class Ability
{
public:
	virtual bool useAbility() { return false; }
	virtual int get_animationType() { return 0; }
	virtual int get_animationNum() { return 0; }
	virtual int get_animationDegradationRate() { return 0; }
	Entity* ownerEntity;
protected:
	vector<Entity*>* vector_entities;
	int animationType, animationNum, animationDegradationRate;
	float manaRequired = 0;
};

#endif