#include "Player.h"
#include "constants.cpp"
using namespace std;

Player::Player()
{

}

Player::Player (int clipX_in, int clipY_in)
{
	//this->textureNum = textureNum_in;
	this->active = true;

	this->mass = 9;
	this->forceX = 0;
	this->forceY = 0;
	this->velX = 0;
	this->velY = 0;
	this->posX = 100;
	this->posY = 100;
	this->rect.x = clipX_in;
	this->rect.y = clipY_in;
	this->rect.w = ENTITY_CLIP_W;
	this->rect.h = ENTITY_CLIP_H;

	for(int i = 0; i < 4; i++)
	{
		this->keyDown[i] = false;
	}
}

void Player::pressKey(int toPress)
{
	this->keyDown[toPress] = true;
}

void Player::releaseKey(int toRelease)
{
	this->keyDown[toRelease] = false;
}

bool Player::keyIsDown(int i)
{
	return keyDown[i];
}