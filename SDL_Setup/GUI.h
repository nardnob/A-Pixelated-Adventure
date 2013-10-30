#ifndef GUI_H
#define GUI_H

#include "Gamestate.h"
#include "HUD.h"
#include "TerrainMap.h"

#include <string>
using namespace std;


class GUI
{
private:
	Gamestate* gamestatePtr;
public:
	//teleports the player 0 to (x, y) in the in_direction
	void teleport(double in_x, double in_y, int in_direction = -1);

	//switches from one map to another
	void switchMap(string in_mapFileName, double in_x, double in_y, TerrainMap& currentMap, HUD& hud);

	GUI();
	GUI(Gamestate* in_gamestate);
};

#endif