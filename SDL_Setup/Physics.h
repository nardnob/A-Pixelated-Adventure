#ifndef PHYSICS_H
#define PHYSICS_H
#include "Boundary.h"
#include "HUD.h"
#include "Player.h"
#include <vector>
using namespace std;

class Physics 
{
private:

public:
	static void doPhysics(vector<Player>& inVector, HUD& hud, vector<Boundary>& boundaries);
};

#endif