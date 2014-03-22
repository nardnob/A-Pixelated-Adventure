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
	font_Gamestate_2 = TTF_OpenFont(FONT_GAMESTATE_2_FILENAME, FONT_GAMESTATE_2_SIZE);
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

	//Set the games current state to the death menu.
	this->currentState = newState;

	//clear the keyboard buffer
	for(int i = 0; i < KEY_BUFFER_SIZE; i++)
	{
		this->vector_players.at(0).releaseKey(i);
	}

	switch(newState)
	{
	case STATES_DEATH_MENU:		
		//reload to map_001 at the starting point
		gui->switchMap("map_001.txt", 100, 100);

		this->vector_menuObjects.push_back(
			new Message(
				0,
				0,
				this->font_Gamestate_1,
				"Gameover! Loser!",
				true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight / 2 - this->vector_menuObjects.back()->get_height() / 2;

		this->vector_menuObjects.push_back(
			new Message(
			0,
			0,
			this->font_Gamestate_2,
			"Press Spacebar to continue ...",
			true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = (gui->monitorHeight * 3)/4 - this->vector_menuObjects.back()->get_height() / 2;

		break;

	case STATES_GAMEPLAY:
		//restore the players' life
		vector_players.at(0).currentStatus.healLife();

		break;
	}
}