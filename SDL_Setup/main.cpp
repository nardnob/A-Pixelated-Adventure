/*
The SDL setup functions used in this code were created by following the lazyfoo tutorials at lazyfoo.net
*/

//The headers
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "constants.cpp"
#include "Entity.h"
#include "Gamestate.h"
#include "GUI.h"
#include "HUD.h"
#include "MenuEngine.h"
#include "Physics.h"
#include "RadiusAttackAbility.h"

//#include <Windows.h> //for OutputDebugString()
#include <string>
#include <vector>
using namespace std;


int main( int argc, char* args[] )
{
	//Initialize SDL_ttf
	if(TTF_Init() == -1)
		return false;

	Gamestate gamestate = Gamestate();	

	GUI gui = GUI(&gamestate);
	gamestate.gui = &gui;

	//define the first player
	gui.defineClip(CODE_PLAYER);

	//the HUD (holds the hud surface(s) and messages
	HUD hud = HUD(); 
	gui.hudPtr = &hud;

	//define the clips (clip up the texture files)
	gui.defineClip(CODE_TERRAIN);

	//SDL's init()
	if( !gui.init() )
        return 1;  

	//to define the HUD and its messages
	gui.defineHUD();

	//SDL load_files to load images
    if( !gui.load_files() )
        return 1;

	//set the offsets of the surface_screen for the first time (to center the screen in the window)
	gui.setScreenOffsets();

	//set up the first currentMap
	gamestate.init();

    //***********************************************************************************
	//*********** The game loop *********************************************************
	//***********************************************************************************
    while( !gui.quit )
    {
		try
		{
			//reset the frame timer to 0. Used to regulate minimum time per frame
			gui.fpsTimer.start();

			//call the eventHandler
			gui.eventHandler();

			//switch through the gamestates for the game engine
			switch(gamestate.currentState)
			{
			case STATES_GAMEPLAY:
				//do some physics
				Physics::doPhysics(gamestate, hud, gui);
				break;
			case STATES_DEATH_MENU:
				MenuEngine::engine(STATES_DEATH_MENU, gamestate);
				break;
			case STATES_START_MENU:
				MenuEngine::engine(STATES_START_MENU, gamestate);
				break;
			case STATES_PAUSE_MENU:
				MenuEngine::engine(STATES_PAUSE_MENU, gamestate);
				break;
			}

			//apply all of the surfaces to surface_screen
			gui.displayAll();

			//Update the screen by flipping surface_screen
			if(!gui.flipScreen())
				return 1;

			//regulate the frame rate, and update the currentFrameRate (needs to stay at end of game loop)
			gui.frameRate();
		}
		//catch(std::exception& e)
		catch(...) //bad-practice attempt at a catch-all to prevent crashes in full-screen
		{
		}
    }
    //***********************************************************************************
	//*********** End of the game loop **************************************************
	//***********************************************************************************

    //Free the images and quit SDL
    gui.clean_up();

    return 0;
}