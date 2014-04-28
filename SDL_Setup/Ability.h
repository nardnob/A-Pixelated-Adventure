#ifndef ABILITY_H
#define ABILITY_H

#include <vector>
using namespace std;

#include "Entity.h"

class Ability
{
public:
	virtual void useAbility() {}
private:
	vector<Entity*>* vector_entities;
};

#endif