#include <string>
#include <vector>
#include <Windows.h>

#include "Button.h"
using namespace std;

Button::Button(vector<int>* in_clickEvents, int in_clickId, int in_rectW, int in_rectH, int in_rectX, int in_rectY, int in_posX, int in_posY)
{
	this->clickEvents = in_clickEvents;
	this->clickId = in_clickId;
	this->buttonRect.w = in_rectW;
	this->buttonRect.h = in_rectH;
	this->buttonRect.x = in_rectX;
	this->buttonRect.y = in_rectY;
	this->posX = in_posX;
	this->posY = in_posY;
}

void Button::defineWidthHeight()
{

}

int Button::get_width()
{
	return this->buttonRect.w;
}

int Button::get_height()
{
	return this->buttonRect.h;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}

void Button::display(SDL_Surface* surface_messager, SDL_Surface* surface_buttons, SDL_Surface* surface_screen)
{
	SDL_Rect temp = this->buttonRect;
	temp.y += this->rectOffset_y;

	apply_surface(
		this->posX,
		this->posY,
		surface_buttons,
		surface_screen,
		&temp);
}

void Button::handleMouseOver(int x, int y)
{
	if(x >= this->posX
	   && x <= this->posX + this->buttonRect.w
	   && y >= this->posY
	   && y <= this->posY + this->buttonRect.h
	   )
	{
		this->mouseOver = true;
	}
	else
	{
		this->mouseOver = false;
	}

	this->updateOffset();
}

void Button::handleMouseDown(int x, int y)
{
	if(x >= this->posX
	   && x <= this->posX + this->buttonRect.w
	   && y >= this->posY
	   && y <= this->posY + this->buttonRect.h
	   )
	{
		this->mouseDown = true;
	}

	this->updateOffset();
}

void Button::handleMouseUp(int x, int y)
{
	if(x >= this->posX
	   && x <= this->posX + this->buttonRect.w
	   && y >= this->posY
	   && y <= this->posY + this->buttonRect.h
	   && this->mouseDown
	   )
	{
		onClick(this->clickId);
	}

	this->mouseDown = false;

	this->updateOffset();
}

void Button::onClick(int clickId)
{
	clickEvents->push_back(clickId);
}

void Button::updateOffset()
{
	if(this->mouseDown)
	{
		this->rectOffset_y = this->buttonRect.h * 2;
	}
	else
		if(this->mouseOver)
		{
			this->rectOffset_y = this->buttonRect.h * 1;
		}
		else
		{
			this->rectOffset_y = this->buttonRect.h * 0;
		}
}