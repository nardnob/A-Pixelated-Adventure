#ifndef BOUNDARY_H
#define BOUNDARY_H

using namespace std;

class Boundary 
{
private:

public:
	int
		x, //the boundary's x (top-left most point)
		y, //the boundary's y (top-left most point)
		w, //the boundary's width
		h; //the boundary's height

	bool active; //whether or not the boundary should be considered in physics collisions

	Boundary();
	Boundary(int in_x, int in_y, int in_w, int in_h, bool in_active);
};

#endif