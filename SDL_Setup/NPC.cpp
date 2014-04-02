#include "NPC.h"
#include "constants.cpp"
using namespace std;

NPC::NPC()
{

}

NPC::NPC(int in_clipX, int in_clipY, double in_posX, double in_posY, double in_base_posX, double in_base_posY, double in_base_w, double in_base_h, double in_base_damage)
{
	this->baseDamage = 1;

	//this->textureNum = textureNum_in;
	this->active = true;

	this->forceX = 0;
	this->forceY = 0;
	this->velX = 0;
	this->velY = 0;

	this->mass = MASS_NPC;
	this->maxVelX = 3;
	this->maxVelY = 3;
	this->posX = in_posX;
	this->posY = in_posY;
	this->base_posX = in_base_posX;
	this->base_posY = in_base_posY;
	this->base_w = in_base_w;
	this->base_h = in_base_h;
	this->maxVel = 2;
	this->baseDamage = in_base_damage;

	this->currentTexture = TEXTURE_FRONT;

	//define TEXTURE_UP, DOWN, LEFT, and RIGHT clips
	for(int i = 0; i < 4; i++)
	{
		this->rect[i].x = in_clipX + ENTITY_CLIP_W * i;
		this->rect[i].y = in_clipY;
		this->rect[i].w = ENTITY_CLIP_W;
		this->rect[i].h = ENTITY_CLIP_H;
	}
}

//should only accept a constant, i.e. TEXTURE_RIGHT, TEXTURE_LEFT. Must not overreach the bounds of the rect[] (currentTexture will be used as rect[currentTexture] in display)
void NPC::toggleTexture(int toChoose)
{
	this->currentTexture = toChoose;
}

float NPC::playerCollisionDamage()
{
	//if the attack speed cooldown has passed since the last attack.. need to work on
	return this->baseDamage;
}