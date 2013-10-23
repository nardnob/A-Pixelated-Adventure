#ifndef ENTITY_H
#define ENTITY_H
#include "SDL.h"
#include "SDL_image.h"

class Entity //will probably want to make this abstract
{
private:

protected:
	bool active; //whether or not the texture is displaying
	int currentTexture;
	//int textureNum; //the indices of this Entity's texture in the textureClips array // deleted this since I'm now storing textures per-object instead of in an array

public:
	int	posX, //the x to display this Entity's texture at if the Entity is active
		posY; //the y to display this Entity's texture at if the Entity is active

	SDL_Rect rect[4]; //to hold the clip(s) (of the textures)

	Entity();

	int get_currentTexture() { return this->currentTexture; }

	/*int get_posX() { return this->posX; } // made posX and posY public for testing purposes. Will change back to private
	void set_posX(int posX_in) { this->posX = posX_in; }

	int get_posY() { return this->posY; }
	void set_posY(int posY_in) { this->posY = posY_in; }*/
};

#endif