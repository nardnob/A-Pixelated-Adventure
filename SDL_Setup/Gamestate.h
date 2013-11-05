#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Entity.h"
#include "NPC.h"
#include "Player.h"
#include "TerrainMap.h"

#include <vector>
using namespace std;

//class TerrainMap;

class Gamestate
{
private:
public:
	Gamestate();

	TerrainMap currentMap; //the current map object
	vector<Player> vector_players; //vector to hold the Player object(s)
	vector<NPC> vector_NPCs;
	vector<Entity*> vector_entities; //this will hold a vector of all entities (for polymorphism to output all entities in one loop, regardless of specific types)
	
	//set up the first currentMap
	void init();
};

#endif