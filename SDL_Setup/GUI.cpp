#include "constants.cpp"
#include "GUI.h"
#include "MapDoor_Boundary.h"
#include <string>
using namespace std;

GUI::GUI()
{

}

GUI::GUI(Gamestate* in_gamestate) 
{ 
	this->gamestatePtr = in_gamestate; 
}

void GUI::switchMap(string in_mapFileName, double in_x, double in_y, TerrainMap& currentMap, HUD& hud)
{
	currentMap = TerrainMap(in_mapFileName);

	string temp = "MAP: " + currentMap.get_mapFileName();
	teleport(in_x, in_y);
	/*
	//defineHUD(currentMap.get_sizeX() * TERRAIN_CLIP_W, currentMap.get_sizeY() * TERRAIN_CLIP_H, currentMap, hud);
	hud.HUD_rect.w = currentMap.get_sizeX() * TERRAIN_CLIP_W;
	hud.HUD_rect.h = HUD_HEIGHT;
	hud.HUD_rect.x = 0;
	hud.HUD_rect.y = currentMap.get_sizeY() * TERRAIN_CLIP_H;*/
	temp = "MAP: " + currentMap.get_mapFileName();

	hud.advancedMessages.at(hud.MESSAGE_CURRENTMAP).set_message(temp.c_str());

	//need to set new boundary at this x and y to not 
}

//teleports player 0 to position (x, y) and faces them in the in_direction if specified
void GUI::teleport(double in_x, double in_y, int in_direction)
{
	gamestatePtr->vector_players.at(0).posX = in_x;
	gamestatePtr->vector_players.at(0).posY = in_y;

	if(in_direction >= 0)
		gamestatePtr->vector_players.at(0).set_currentTexture(in_direction);
}