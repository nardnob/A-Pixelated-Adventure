#include "PlayerStatus.h"
using namespace std;

//Initialize PlayerStatus' members to determined maximums.
PlayerStatus::PlayerStatus()
{
	this->life = 100; //Should be set to a determined maximum
	//this->life = stats.get_maxLife();
}

//Give x life to the player (up to a determined maximum).
void PlayerStatus::giveLife(float x)
{
	this->life += x;
	if(this->life > 100) //if(this->life > this->stats.get_maxLife();
		this->life = 100;
}

//Does the player have > 0 life?
bool PlayerStatus::hasLife()
{
	if(this->life > 0)
		return true;
	return false;
}

//Heal the player's life. Health set to a determined maximum.
void PlayerStatus::healLife()
{
	this->life = 100;
	//this->life = stats.get_maxLife();
}

//Take x life from the player (down to a minimum of 0).
void PlayerStatus::takeLife(float x)
{
	this->life -= x;
	if(this->life < 0)
		this->life = 0;
}

float PlayerStatus::lifeAmount() //Returns the amount of life.
{
	return this->life;
}