#ifndef PLAYERSTATUS_H
#define PLAYERSTATUS_H

class PlayerStatus
{
private:


	//pointer to the player's stats

public:
	PlayerStatus(); //Initialize PlayerStatus' members to determined maximums.

	float
		life, //The player's current life.
		maxLife, //The player's maximum life.
		mana,
		maxMana;

	bool hasLife(); //Does the player have > 0 life?
	bool hasMana(float x); //Does the player have x mana to take?
	void giveLife(float); //Give x life to the player (up to a determined maximum).
	void takeLife(float); //Take x life from the player (down to a minimum of 0).
	void giveMana(float);
	void takeMana(float);
	void healLife(); //Heal the player's life. health set to a determined maximum.
	void healMana(); //Heal the player's mana. mana set to a determined maximum.
	float lifeAmount(); //Returns the amount of life.
	float lifePercent(); //Returns the ratio of life/maxLife
	float manaAmount();
	float manaPercent();
};

#endif