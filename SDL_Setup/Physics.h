#ifndef PHYSICS_H
#define PHYSICS_H
#include "Player.h"
#include <vector>
using namespace std;

class Physics 
{
private:

public:
	static void doPhysics(vector<Player>& inVector);
};

#endif