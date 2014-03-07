#include "Boundary.h"
using namespace std;

//just in case someone feels the need to create a boundary for future use? I guess?
Boundary::Boundary()
{
	this->x = 0;
	this->y = 0;
	this->w = 0;
	this->h = 0;
	this->active = false;
}

Boundary::Boundary(int in_x, int in_y, int in_w, int in_h, bool in_active)
{
	this->x = in_x;
	this->y = in_y;
	this->w = in_w;
	this->h = in_h;
	this->active = in_active;
}