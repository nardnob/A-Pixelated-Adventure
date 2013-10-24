#ifndef MESSAGE_H
#define MESSAGE_H

#include "SDL.h"
#include "SDL_ttf.h"
#include <string>
using namespace std;

class Message
{
private:	
	int
		posX,
		posY;

	TTF_Font* font;

	string message;

	bool active;
public:
	Message();
	Message(int in_posX, int in_posY, TTF_Font* in_font, char* in_message); //defaults active to false
	Message(int in_posX, int in_posY, TTF_Font* in_font, char* in_message, bool in_active);
	
	int get_posX() { return this->posX; }
	void set_posX(int in_posX) { this->posX = in_posX; }

	int get_posY() { return this->posY; }
	void set_posY(int in_posY) { this->posY = in_posY; }

	TTF_Font* get_font() { return this->font; }
	void set_font(TTF_Font* in_font) { this->font = in_font; }

	bool get_active() { return this->active; }
	void set_active(bool in_active) { this->active = in_active; }

	const char* get_message() { return this->message.c_str(); }
	void set_message(string in_message) { this->message = in_message; }
};

#endif