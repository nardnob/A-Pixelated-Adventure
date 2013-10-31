#include "MapDoor_Boundary.h"

//just in case someone feels the need to create a boundary for future use? I guess?
MapDoor_Boundary::MapDoor_Boundary()
{
	this->x = 0;
	this->y = 0;
	this->w = 0;
	this->h = 0;
	this->active = false;
	
	this->toMap = "";
	this->toX = 0;
	this->toY = 0;
}

MapDoor_Boundary::MapDoor_Boundary(int in_x, int in_y, int in_w, int in_h, bool in_active, string in_toMap, double in_toX, double in_toY)
{
	this->x = in_x;
	this->y = in_y;
	this->w = in_w;
	this->h = in_h;
	this->active = in_active;

	this->toMap = in_toMap;
	this->toX = in_toX;
	this->toY = in_toY;

	this->inTheDoorway = true;
}