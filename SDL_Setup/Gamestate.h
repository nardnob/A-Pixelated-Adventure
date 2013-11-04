#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Player.h"
#include "NPC.h"

#include <vector>
using namespace std;

class Gamestate
{
private:

public:
	vector<Player> vector_players; //vector to hold the Player object(s)
	vector<NPC> vector_NPCs;
	vector<Entity*> vector_entities; //this will hold a vector of all entities (for polymorphism to output all entities in one loop, regardless of specific types)
};

#endif