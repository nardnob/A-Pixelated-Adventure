#include "constants.cpp"
#include "Message.h"
#include "Gamestate.h"
using namespace std;

Gamestate::Gamestate()
{
	//The current state of the game to be calculated / displayed. i.e. Gameplay, menu system, etc.
	currentState = STATES_GAMEPLAY;

	//open the Gamestate font
	font_Gamestate_1 = TTF_OpenFont(FONT_GAMESTATE_1_FILENAME, FONT_GAMESTATE_1_SIZE);
}

void Gamestate::init()
{	
	//initialize the currentMap object with the mapFileName text file
	currentMap = TerrainMap("map_001.txt", this);
}

void Gamestate::switchState(int newState)
{
	//clear the current menuObjects
	this->vector_menuObjects.clear();

	switch(newState)
	{
	case STATES_DEATH_MENU:
		this->currentState = STATES_DEATH_MENU;
		this->vector_menuObjects.push_back(
			new Message(
				0,
				0,
				this->font_Gamestate_1,
				"Gameover! Dumbass!",
				true
			));

		vector_menuObjects.back()->defineWidthHeight();

		this->vector_menuObjects.back()->posX = *monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = *monitorHeight  / 2 - this->vector_menuObjects.back()->get_height() / 2;
		break;
	}
}