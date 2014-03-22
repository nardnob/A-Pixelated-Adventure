#include "MenuEngine.h"

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
	}
}