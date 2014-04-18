#ifndef MENU_OBJECT
#define MENU_OBJECT

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

class MenuObject
{
public:
	int
		posX,
		posY;
	
	int get_posX() { return this->posX; }
	int get_posY() { return this->posY; }
	void set_posX(int in_posX) { this->posX = in_posX; }
	void set_posY(int in_posY) { this->posY = in_posY; }

	virtual void defineWidthHeight() {};
	virtual int get_width() { return 1; };
	virtual int get_height() { return 1; };
	virtual void display(SDL_Surface* surface_messager, SDL_Surface* surface_screen) {}
	virtual void handleMouseOver() {}
	virtual void handleMouseClick() {}
private:

};

#endif