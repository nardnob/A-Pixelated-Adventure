#ifndef MAPDOOR_BOUNDARY_H
#define MAPDOOR_BOUNDARY_H
#include "Boundary.h"
#include <string>
using std::string;

class MapDoor_Boundary: public Boundary
{
private:

public:
	string toMap; //the map file name to be loaded upon collision with the boundary
	double toX, toY; //the x and y to be teleported to once the map is loaded
	bool 
		inTheDoorway, //if you're in the doorway and have already resolved doorway collisions, don't resolve until you leave
		vertical; //if the door is on the left or right wall, it is vertical, else it is not (it is horizontal on the top and bottom).

	MapDoor_Boundary();
	MapDoor_Boundary(int in_x, int in_y, int in_w, int in_h, bool in_active, string in_toMap, double in_toX, double in_toY);
};

#endif