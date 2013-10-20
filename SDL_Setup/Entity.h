#ifndef ENTITY_H
#define ENTITY_H
#include "SDL.h"
#include "SDL_image.h"

class Entity
{
private:

protected:
	bool active; //whether or not the texture is displaying
	int textureNum; //the indices of this Entity's texture in the textureClips array

public:
	int	posX, //the x to display this Entity's texture at if the Entity is active
		posY; //the y to display this Entity's texture at if the Entity is active

	SDL_Rect rect; //to hold the clip(s) (of the textures)

	Entity();
	Entity(int textureNum_in);

	/*int get_posX() { return this->posX; }
	void set_posX(int posX_in) { this->posX = posX_in; }

	int get_posY() { return this->posY; }
	void set_posY(int posY_in) { this->posY = posY_in; }*/

	int get_textureNum() { return this->textureNum; }
	void set_textureNum(int textureNum_in) { this->textureNum = textureNum_in; }
};

#endif