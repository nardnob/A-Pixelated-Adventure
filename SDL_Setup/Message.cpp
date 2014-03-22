#include "constants.cpp"
#include "Message.h"
using namespace std;

Message::Message()
{
	this->width = 0;
	this->height = 0;
}

Message::Message(int in_posX, int in_posY, TTF_Font* in_font, char* in_message, bool in_active)
{
	this->posX = in_posX;
	this->posY = in_posY;
	this->font = in_font;
	this->active = in_active;
	this->message = in_message;
	this->width = 0;
	this->height = 0;

	defineWidthHeight();
}

Message::Message(int in_posX, int in_posY, TTF_Font* in_font, char* in_message)
{
	this->posX = in_posX;
	this->posY = in_posY;
	this->font = in_font;
	this->active = false;
	this->message = in_message;
	this->width = 0;
	this->height = 0;
}

void Message::apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}

void Message::display(SDL_Surface* surface_messager, SDL_Surface* surface_screen)
{
	surface_messager = TTF_RenderText_Solid(this->font, this->message.c_str(), FONT_COLOR_WHITE);

	apply_surface(
		this->posX,
		this->posY,
		surface_messager,
		surface_screen);

	SDL_FreeSurface(surface_messager);
}

void Message::defineWidthHeight()
{
	SDL_Surface* temp = TTF_RenderText_Solid(this->font, this->message.c_str(), FONT_COLOR_WHITE);

	this->width = temp->w;
	this->height = temp->h;

	SDL_FreeSurface(temp);
}

int Message::get_width()
{
	return this->width;
}

int Message::get_height()
{
	return this->height;
}


bool Message::mouseOver()
{

	return false;
}

bool Message::mouseClick()
{

	return false;
}