#include "PlayerStatus.h"
using namespace std;

//Initialize PlayerStatus' members to determined maximums.
PlayerStatus::PlayerStatus()
{
	this->life = 100; //Should be set to a determined maximum
	//this->life = stats.get_maxLife();
	this->maxLife = 100; //should be set to a determined maximum
	this->mana = 100;
	this->maxMana = 100;
}

//Give x life to the player (up to a determined maximum).
void PlayerStatus::giveLife(float x)
{
	this->life += x;
	if(this->life > this->maxLife) //if(this->life > this->stats.get_maxLife();
		this->life = this->maxLife;
}

void PlayerStatus::giveMana(float x)
{
	this->mana += x;
	if(this->mana > this->maxMana)
		this->mana = this->maxMana;
}

//Does the player have > 0 life?
bool PlayerStatus::hasLife()
{
	if(this->life > 0)
		return true;
	return false;
}

bool PlayerStatus::hasMana(float x = 0)
{
	if(this->mana - x >= 0)
		return true;
	return false;
}

//Heal the player's life. Health set to a determined maximum.
void PlayerStatus::healLife()
{
	this->life = this->maxLife;
	//this->life = stats.get_maxLife();
}

void PlayerStatus::healMana()
{
	this->mana = this->maxMana;
}

//Take x life from the player (down to a minimum of 0).
void PlayerStatus::takeLife(float x)
{
	this->life -= x;
	if(this->life < 0)
		this->life = 0;
}

void PlayerStatus::takeMana(float x)
{
	this->mana -= x;
	if(this->mana < 0)
		this->mana = 0;
}

float PlayerStatus::lifeAmount() //Returns the amount of life.
{
	return this->life;
}

float PlayerStatus::manaAmount()
{
	return this->mana;
}

float PlayerStatus::lifePercent()
{
	return this->life / this->maxLife;
}

float PlayerStatus::manaPercent()
{
	return this->mana / this->maxMana;
}