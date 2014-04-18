#ifndef MESSAGE_H
#define MESSAGE_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "MenuObject.h"
#include <string>
using namespace std;

class Message : public MenuObject
{
private:
	TTF_Font* font;

	string message;

	int width, height;

	bool active;
public:
	Message();
	Message(int in_posX, int in_posY, TTF_Font* in_font, char* in_message); //defaults active to false
	Message(int in_posX, int in_posY, TTF_Font* in_font, char* in_message, bool in_active);

	//display the message at (x,y) with it's message and font
	void display(SDL_Surface* surface_messager, SDL_Surface* surface_buttons, SDL_Surface* surface_screen);

	void handleMouseOver(int x, int y);
	void handleMouseDown(int x, int y);
	void handleMouseUp(int x, int y);

	int get_width();
	int get_height();

	void defineWidthHeight();

	TTF_Font* get_font() { return this->font; }
	void set_font(TTF_Font* in_font) { this->font = in_font; }

	bool get_active() { return this->active; }
	void set_active(bool in_active) { this->active = in_active; }

	const char* get_message() { return this->message.c_str(); }
	void set_message(string in_message) { this->message = in_message; }
	
	void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
};

#endif