#include "Animation.h"

void Animation::display(SDL_Surface* surface_animations, SDL_Surface* surface_screen, double screenOffset_x, double screenOffset_y)
{
	apply_surface(
		this->posX + screenOffset_x,
		this->posY + screenOffset_y,
		surface_animations,
		surface_screen,
		&this->animationClip);
}

void Animation::apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}