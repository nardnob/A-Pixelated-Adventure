#ifndef PHYSICS_H
#define PHYSICS_H
#include "Player.h"
#include "HUD.h"
#include <vector>
using namespace std;

class Physics 
{
private:

public:
	static void doPhysics(vector<Player>& inVector, HUD& hud);
};

#endif