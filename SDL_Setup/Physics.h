#ifndef PHYSICS_H
#define PHYSICS_H
#include "Boundary.h"
#include "Gamestate.h"
#include "GUI.h"
#include "HUD.h"
#include "Player.h"
#include "TerrainMap.h"
#include <vector>
using namespace std;

class Physics 
{
private:

public:
	static void doPhysics(Gamestate& gamestate, HUD& hud, vector<Boundary>& boundaries, vector<MapDoor_Boundary>& mapDoor_boundaries, GUI& gui, TerrainMap& currentMap);
};

#endif