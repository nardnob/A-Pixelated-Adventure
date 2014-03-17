#ifndef PLAYERSTATUS_H
#define PLAYERSTATUS_H

class PlayerStatus
{
private:
	float
		life; //The player's current life.

	//pointer to the player's stats

public:
	PlayerStatus(); //Initialize PlayerStatus' members to determined maximums.

	bool hasLife(); //Does the player have > 0 life?
	void giveLife(float); //Give x life to the player (up to a determined maximum).
	void takeLife(float); //Take x life from the player (down to a minimum of 0).
	void healLife(); //Heal the player's life. health set to a determined maximum.
};

#endif