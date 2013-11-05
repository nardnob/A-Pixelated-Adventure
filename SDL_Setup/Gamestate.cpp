#include "Gamestate.h"
using namespace std;

Gamestate::Gamestate()
{
}

void Gamestate::init()
{	
	//initialize the currentMap object with the mapFileName text file
	currentMap = TerrainMap("map_001.txt", this);
}