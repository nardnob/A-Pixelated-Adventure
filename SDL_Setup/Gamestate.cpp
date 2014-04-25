#include <Windows.h>

#include "constants.cpp"
#include "Button.h"
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
	font_Gamestate_3 = TTF_OpenFont(FONT_GAMESTATE_3_FILENAME, FONT_GAMESTATE_3_SIZE);
}

void Gamestate::init()
{	
	//initialize the currentMap object with the mapFileName text file
	//currentMap = TerrainMap("map_001.txt", this);
	this->switchState(STATES_START_MENU);
//	gui->switchMap("map_001.txt", 100, 100);
	//vector_players.at(0).currentStatus.healLife();
}

void Gamestate::switchState(int newState)
{
	int prevState = this->currentState;

	//delete the memory spaces in the heap
	for(int i = 0; i < this->vector_menuObjects.size(); i++)
	{
		delete this->vector_menuObjects.at(i);
	}

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
	case STATES_OPTIONS_MENU:
		this->vector_menuObjects.push_back(
			new Message(
			0,
			0,
			this->font_Gamestate_1,
			"Options",
			true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight / 5 - this->vector_menuObjects.back()->get_height() / 2;

		this->vector_menuObjects.push_back(
			new Message(
			0,
			0,
			this->font_Gamestate_3,
			"Under Construction",
			true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight / 2 - this->vector_menuObjects.back()->get_height() / 2;

		/*
		this->vector_menuObjects.push_back(
			new Message(
			0,
			0,
			this->font_Gamestate_2,
			"Press Spacebar to continue ...",
			true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = (gui->monitorHeight * 7) / 8 - this->vector_menuObjects.back()->get_height() / 2;
		*/

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_TOSTATE_PREVIOUS,
			BUTTON_1_WIDTH,
			BUTTON_1_HEIGHT,
			0,
			0,
			0,
			0,
			"Back",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2 + 300;

		break;
	case STATES_CREDITS_MENU:
		this->vector_menuObjects.push_back(
			new Message(
			0,
			0,
			this->font_Gamestate_1,
			"Credits",
			true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight / 5 - this->vector_menuObjects.back()->get_height() / 2;

		this->vector_menuObjects.push_back(
			new Message(
			0,
			0,
			this->font_Gamestate_3,
			"Brandon Dixson - Lead Developer",
			true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - 250;// -this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = /*(gui->monitorHeight * 3) / 4 - this->vector_menuObjects.back()->get_height() / 2 -*/ 500;

		this->vector_menuObjects.push_back(
			new Message(
			0,
			0,
			this->font_Gamestate_3,
			"Brooks Wright  - Business Manager & Level Design",
			true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - 250;// -this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = /*(gui->monitorHeight * 3) / 4 - this->vector_menuObjects.back()->get_height() / 2 -*/ 600;

		this->vector_menuObjects.push_back(
			new Message(
			0,
			0,
			this->font_Gamestate_3,
			"David Loveless - Created Base Entity Sprites",
			true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - 250;// -this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = /*(gui->monitorHeight * 3) / 4 - this->vector_menuObjects.back()->get_height() / 2 -*/ 700;
		
		/*
		this->vector_menuObjects.push_back(
			new Message(
			0,
			0,
			this->font_Gamestate_2,
			"Press Spacebar to continue ...",
			true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = (gui->monitorHeight * 7) / 8 - this->vector_menuObjects.back()->get_height() / 2;
		*/

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_TOSTATE_STARTMENU,
			BUTTON_1_WIDTH,
			BUTTON_1_HEIGHT,
			0,
			0,
			0,
			0,
			"Back",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2 + 300;
		break;

	case STATES_PAUSE_MENU:
		this->vector_menuObjects.push_back(
			new Message(
			0,
			0,
			this->font_Gamestate_1,
			"Paused",
			true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight / 5 - this->vector_menuObjects.back()->get_height() / 2;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_TOSTATE_GAMEPLAY,
			BUTTON_1_WIDTH,
			BUTTON_1_HEIGHT,
			0,
			0,
			0,
			0,
			"Resume (spacebar)",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_TOSTATE_OPTIONS,
			BUTTON_1_WIDTH,
			BUTTON_1_HEIGHT,
			0,
			0,
			0,
			0,
			"Options",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2 + 100;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_TOSTATE_STARTMENU,
			BUTTON_1_WIDTH,
			BUTTON_1_HEIGHT,
			0,
			0,
			0,
			0,
			"Main Menu",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2 + 200;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_EXIT,
			BUTTON_1_WIDTH,
			BUTTON_1_HEIGHT,
			0,
			0,
			0,
			0,
			"Exit Game",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2 + 300;
		break;
	case STATES_DEATH_MENU:		
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
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_TOSTATE_STARTMENU,
			BUTTON_1_WIDTH,
			BUTTON_1_HEIGHT,
			0,
			0,
			0,
			0,
			"Return to the Main Menu",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 7 / 8 - this->vector_menuObjects.back()->get_height() / 2;

		/*
		this->vector_menuObjects.push_back(
			new Message(
				0,
				0,
				this->font_Gamestate_2,
				"Press Spacebar to continue ...",
				true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = (gui->monitorHeight * 7) / 8 - this->vector_menuObjects.back()->get_height() / 2;
		*/
		break;

	case STATES_GAMEPLAY:
		if(prevState != STATES_PAUSE_MENU)
		{
			//reload to map_001 at the starting point
			gui->switchMap("map_001.txt", 100, 100);

			//restore the players' life
			vector_players.at(0).currentStatus.healLife();

			this->resetGameplay = false;
		}
		break;

	case STATES_START_MENU:
		this->vector_menuObjects.push_back(
			new Button(
				&this->vector_clickEvents,
				Button::BUTTON_TOSTATE_GAMEPLAY,
				BUTTON_1_WIDTH,
				BUTTON_1_HEIGHT,
				0,
				0,
				0,
				0,
				"Start the Game",
				this->font_Gamestate_3
				));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_TOSTATE_OPTIONS,
			BUTTON_1_WIDTH,
			BUTTON_1_HEIGHT,
			0,
			0,
			0,
			0,
			"Options",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2 + 100;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_TOSTATE_CREDITS,
			BUTTON_1_WIDTH,
			BUTTON_1_HEIGHT,
			0,
			0,
			0,
			0,
			"Credits",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2 + 200;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_EXIT,
			BUTTON_1_WIDTH,
			BUTTON_1_HEIGHT,
			0,
			0,
			0,
			0,
			"Exit",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2 + 300;

		break;
	}
}