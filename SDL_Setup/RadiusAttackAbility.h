#ifndef RADIUSATTACKABILITY_H
#define RADIUSATTACKABILITY_H

class RadiusAttackAbility
{
private:
	float
		radius, //the radius of the radius-based attack
		baseDamage; //the base damage of the attack
public:
	RadiusAttackAbility(float in_radius, float in_baseDamage);
	void useAbility();
};

#endif