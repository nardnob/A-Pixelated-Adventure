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
		maxLife; //The player's maximum life.

	bool hasLife(); //Does the player have > 0 life?
	void giveLife(float); //Give x life to the player (up to a determined maximum).
	void takeLife(float); //Take x life from the player (down to a minimum of 0).
	void healLife(); //Heal the player's life. health set to a determined maximum.
	float lifeAmount(); //Returns the amount of life.
	float lifePercent(); //Returns the ratio of life/maxLife
};

#endif