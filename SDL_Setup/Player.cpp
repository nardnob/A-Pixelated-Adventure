#include "Player.h"
#include "constants.cpp"
#include "PlayerStatus.h"
using namespace std;

Player::Player()
{

}

Player::Player(int in_clipX, int in_clipY, double in_posX, double in_posY, double in_base_posX, double in_base_posY, double in_base_w, double in_base_h, Player* in_player)
{
	currentStatus = PlayerStatus(in_player);

	//this->textureNum = textureNum_in;
	this->active = true;

	this->forceX = 0;
	this->forceY = 0;
	this->velX = 0;
	this->velY = 0;

	this->mass = MASS_PLAYER;
	this->maxVelX = 3;
	this->maxVelY = 3;
	this->maxVel = 4;
	this->posX = in_posX;
	this->posY = in_posY;
	this->base_posX = in_base_posX;
	this->base_posY = in_base_posY;
	this->base_w = in_base_w;
	this->base_h = in_base_h;

	this->currentTexture = TEXTURE_FRONT;

	//define TEXTURE_UP, DOWN, LEFT, and RIGHT clips
	for(int i = 0; i < 4; i++)
	{
		this->rect[i].x = in_clipX + ENTITY_CLIP_W * i;
		this->rect[i].y = in_clipY;
		this->rect[i].w = ENTITY_CLIP_W;
		this->rect[i].h = ENTITY_CLIP_H;
	}

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

//should only accept a constant, i.e. TEXTURE_RIGHT, TEXTURE_LEFT. Must not overreach the bounds of the rect[] (currentTexture will be used as rect[currentTexture] in display)
void Player::toggleTexture(int toChoose)
{
	this->currentTexture = toChoose;
}