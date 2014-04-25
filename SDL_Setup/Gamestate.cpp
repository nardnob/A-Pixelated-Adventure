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
}

void Gamestate::init()
{	
	//initialize the currentMap object with the mapFileName text file
	//currentMap = TerrainMap("map_001.txt", this);
	this->switchState(STATES_START_MENU);
//	gui->switchMap("map_001.txt", 100, 100);
	//vector_players.at(0).currentStatus.healLife();
}

void Gamestate::defineFont()
{
	if(this->gui->smallMonitor)
	{
		//open the Gamestate font
		font_Gamestate_1 = TTF_OpenFont(FONT_GAMESTATE_1_FILENAME, FONT_GAMESTATE_1_SIZE_SMALL);
		font_Gamestate_2 = TTF_OpenFont(FONT_GAMESTATE_2_FILENAME, FONT_GAMESTATE_2_SIZE_SMALL);
		font_Gamestate_3 = TTF_OpenFont(FONT_GAMESTATE_3_FILENAME, FONT_GAMESTATE_3_SIZE_SMALL);
	}
	else
	{
		//open the Gamestate font
		font_Gamestate_1 = TTF_OpenFont(FONT_GAMESTATE_1_FILENAME, FONT_GAMESTATE_1_SIZE);
		font_Gamestate_2 = TTF_OpenFont(FONT_GAMESTATE_2_FILENAME, FONT_GAMESTATE_2_SIZE);
		font_Gamestate_3 = TTF_OpenFont(FONT_GAMESTATE_3_FILENAME, FONT_GAMESTATE_3_SIZE);
	}
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
			this->gui->button_1_width,
			this->gui->button_1_height,
			0,
			0,
			0,
			0,
			"Back",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight - this->vector_menuObjects.back()->get_height() * 1.25;// / 2;

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
		this->vector_menuObjects.back()->posX = gui->monitorWidth * 3 / 8;
		this->vector_menuObjects.back()->posY = gui->monitorHeight / 2 - this->vector_menuObjects.back()->get_height() / 2 * 5;
		
		this->vector_menuObjects.push_back(
			new Message(
			0,
			0,
			this->font_Gamestate_3,
			"Brooks Wright  - Business Manager & Level Design",
			true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth * 3 / 8;
		this->vector_menuObjects.back()->posY = gui->monitorHeight / 2 - this->vector_menuObjects.back()->get_height() / 2 * 1;

		this->vector_menuObjects.push_back(
			new Message(
			0,
			0,
			this->font_Gamestate_3,
			"David Loveless - Created Base Entity Sprites",
			true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth * 3 / 8;
		this->vector_menuObjects.back()->posY = gui->monitorHeight / 2 + this->vector_menuObjects.back()->get_height() / 2 * 3;
		
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
			this->gui->button_1_width,
			this->gui->button_1_height,
			0,
			0,
			0,
			0,
			"Back",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight - this->vector_menuObjects.back()->get_height() * 1.25;// / 2;
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
			this->gui->button_1_width,
			this->gui->button_1_height,
			0,
			0,
			0,
			0,
			"Resume (spacebar)",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight - this->vector_menuObjects.back()->get_height() * 1.25 * 4;// / 2;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_TOSTATE_OPTIONS,
			this->gui->button_1_width,
			this->gui->button_1_height,
			0,
			0,
			0,
			0,
			"Options",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight - this->vector_menuObjects.back()->get_height() * 1.25 * 3;// / 2;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_TOSTATE_STARTMENU,
			this->gui->button_1_width,
			this->gui->button_1_height,
			0,
			0,
			0,
			0,
			"Main Menu",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight - this->vector_menuObjects.back()->get_height() * 1.25 * 2;// / 2;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_EXIT,
			this->gui->button_1_width,
			this->gui->button_1_height,
			0,
			0,
			0,
			0,
			"Exit Game",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight - this->vector_menuObjects.back()->get_height() * 1.25 * 1;// / 2;
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
			this->gui->button_1_width,
			this->gui->button_1_height,
			0,
			0,
			0,
			0,
			"Return to the Main Menu",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight - this->vector_menuObjects.back()->get_height() * 1.25;// / 2;

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
				this->gui->button_1_width,
				this->gui->button_1_height,
				0,
				0,
				0,
				0,
				"Start the Game",
				this->font_Gamestate_3
				));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight - this->vector_menuObjects.back()->get_height() * 1.25 * 4;// / 2;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_TOSTATE_OPTIONS,
			this->gui->button_1_width,
			this->gui->button_1_height,
			0,
			0,
			0,
			0,
			"Options",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight - this->vector_menuObjects.back()->get_height() * 1.25 * 3;// / 2;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_TOSTATE_CREDITS,
			this->gui->button_1_width,
			this->gui->button_1_height,
			0,
			0,
			0,
			0,
			"Credits",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight - this->vector_menuObjects.back()->get_height() * 1.25 * 2;// / 2;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_EXIT,
			this->gui->button_1_width,
			this->gui->button_1_height,
			0,
			0,
			0,
			0,
			"Exit",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight - this->vector_menuObjects.back()->get_height() * 1.25 * 1;// / 2;

		break;
	}
}