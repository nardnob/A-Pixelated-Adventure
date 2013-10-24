#include "Message.h"
using namespace std;

Message::Message()
{

}

Message::Message(int in_posX, int in_posY, TTF_Font* in_font, char* in_message, bool in_active)
{
	this->posX = in_posX;
	this->posY = in_posY;
	this->font = in_font;
	this->active = in_active;
	this->message = in_message;
}

Message::Message(int in_posX, int in_posY, TTF_Font* in_font, char* in_message)
{
	this->posX = in_posX;
	this->posY = in_posY;
	this->font = in_font;
	this->active = false;
	this->message = in_message;
}