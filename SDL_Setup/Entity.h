#ifndef ENTITY_H
#define ENTITY_H
#include "SDL.h"
#include "SDL_image.h"

#include "PlayerStatus.h"

class Entity //will probably want to make this abstract
{
private:

protected:
	bool active; //whether or not the texture is displaying
	int currentTexture;
	//int textureNum; //the indices of this Entity's texture in the textureClips array // deleted this since I'm now storing textures per-object instead of in an array

public:
	double
		posX, //the x to display this Entity's texture at if the Entity is active
		posY, //the y to display this Entity's texture at if the Entity is active
		base_posX, //the entity's base x position (offset from posX) used for collision calculations
		base_posY, //the entity's base y position (offset from posY) used for collision calculations
		base_w, //the width of the entity's base
		base_h, //the height of the entity's base
		velX,
		velY,
		forceX,
		forceY,
		mass,
		maxVelX,
		maxVelY,
		maxVel;

	SDL_Rect rect[4]; //to hold the clip(s) (of the textures)

	PlayerStatus currentStatus;

	SDL_Rect healthBar; //the green foreground of the health bar; should decrease with health
	SDL_Rect healthBar_BG; //the red background of the health bar
	SDL_Rect healthBar_border; //the gray border for the healthbar

	Entity();

	virtual float playerCollisionDamage() { return 0.0; }

	int get_currentTexture() { return this->currentTexture; }
	void set_currentTexture(int in_currentTexture) { this->currentTexture = in_currentTexture; }

	/*int get_posX() { return this->posX; } // made posX and posY public for testing purposes. Will change back to private
	void set_posX(int posX_in) { this->posX = posX_in; }

	int get_posY() { return this->posY; }
	void set_posY(int posY_in) { this->posY = posY_in; }*/
};

#endif