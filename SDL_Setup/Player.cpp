#include "Player.h"
#include "constants.cpp"
#include "PlayerStatus.h"
using namespace std;

Player::Player()
{

}

Player::Player(int in_clipX, int in_clipY, double in_posX, double in_posY, double in_base_posX, double in_base_posY, double in_base_w, double in_base_h)
{
	currentStatus = PlayerStatus();

	//this->textureNum = textureNum_in;
	this->active = true;

	this->forceX = 0;
	this->forceY = 0;
	this->velX = 0;
	this->velY = 0; 
	this->mass = MASS_PLAYER;
	this->maxVelX = 2;
	this->maxVelY = 2;
	this->maxVel = sqrt(pow(this->maxVelX, 2) + pow(this->maxVelY, 2));
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

	this->healthBar.w = HEALTHBAR_ENTITY_WIDTH;
	this->healthBar.h = HEALTHBAR_ENTITY_HEIGHT;
	this->healthBar.x = this->posX + HEALTHBAR_ENTITY_OFFSET_X;
	this->healthBar.y = this->posY + HEALTHBAR_ENTITY_OFFSET_Y;

	this->healthBar_BG.w = HEALTHBAR_ENTITY_WIDTH;
	this->healthBar_BG.h = HEALTHBAR_ENTITY_HEIGHT;
	this->healthBar_BG.x = this->posX + HEALTHBAR_ENTITY_OFFSET_X;
	this->healthBar_BG.y = this->posY + HEALTHBAR_ENTITY_OFFSET_Y;

	this->healthBar_border.w = HEALTHBAR_BORDER_ENTITY_WIDTH;
	this->healthBar_border.h = HEALTHBAR_BORDER_ENTITY_HEIGHT;
	this->healthBar_border.x = this->posX + HEALTHBAR_BORDER_ENTITY_OFFSET_X;
	this->healthBar_border.y = this->posY + HEALTHBAR_BORDER_ENTITY_OFFSET_Y;
}

void Player::pressKey(int toPress)
{
	bool startWalking = true;;
	for(int i = 0; i < 4; i++)
	{
		if(keyDown[i])
			startWalking = false;
	}

	this->keyDown[toPress] = true;

	if(startWalking)
	{
		this->walking = true;
		this->toWalk = this->walkSpeed;
		this->walkUpNext = false;
		this->spriteOffsetY = ENTITY_CLIP_H;
	}
}

void Player::releaseKey(int toRelease)
{
	this->keyDown[toRelease] = false;

	bool stopWalking = true;
	for(int i = 0; i < 4; i++)
	{
		if(keyDown[i])
			stopWalking = false;
	}

	if(stopWalking)
	{
		this->walking = false;
		this->spriteOffsetY = 0;
	}
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