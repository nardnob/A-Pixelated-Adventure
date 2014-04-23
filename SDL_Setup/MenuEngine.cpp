#include "MenuEngine.h"
#include "Button.h"

void MenuEngine::engine(int state, Gamestate& gamestate)
{
	switch(state)
	{
	case STATES_DEATH_MENU:
		if(gamestate.vector_players.at(0).keyIsDown(KEY_SPACE))
		{
			gamestate.switchState(STATES_GAMEPLAY);
		}
		break;

	case STATES_PAUSE_MENU:
		if(gamestate.vector_players.at(0).keyIsDown(KEY_SPACE))
		{
			gamestate.resetGameplay = true;
			gamestate.switchState(STATES_GAMEPLAY);
		}
		break;

	case STATES_START_MENU:
		if(gamestate.vector_players.at(0).keyIsDown(KEY_SPACE))
		{
			gamestate.switchState(STATES_GAMEPLAY);
		}

		for(int i = 0; i < gamestate.vector_clickEvents.size(); i++)
		{
			switch(gamestate.vector_clickEvents.at(i))
			{
			case Button::BUTTON_TOSTATE_GAMEPLAY:
				gamestate.switchState(STATES_GAMEPLAY);
				break;

			case Button::BUTTON_OPTIONS:
				break;

			case Button::BUTTON_CREDITS:
				break;

			case Button::BUTTON_EXIT:
				gamestate.gui->quit = true;
				break;
			}
			gamestate.vector_clickEvents.pop_back();
		}
		break;
	}
}