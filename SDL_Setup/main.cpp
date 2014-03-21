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
	gamestate.offsetX = &gui.screenOffset_x;
	gamestate.offsetY = &gui.screenOffset_y;
	gamestate.monitorWidth = &gui.monitorWidth;
	gamestate.monitorHeight = &gui.monitorHeight;

	//define the first player
	gui.defineClip(CODE_PLAYER);

	//the HUD (holds the hud surface(s) and messages
	HUD hud = HUD(); 

	//define the clips (clip up the texture files)
	gui.defineClip(CODE_TERRAIN);
	
	//set up the first currentMap
	gamestate.init();

	//SDL's init()
	if( !gui.init() )
        return 1;  

	//to define the HUD and its messages
	gui.defineHUD(hud);

	//SDL load_files to load images
    if( !gui.load_files() )
        return 1;

	//set the offsets of the surface_screen for the first time (to center the screen in the window)
	gui.setScreenOffsets(hud);

    //***********************************************************************************
	//*********** The game loop *********************************************************
	//***********************************************************************************
    while( !gui.quit )
    {
		//reset the frame timer to 0. Used to regulate minimum time per frame
		gui.fpsTimer.start();

		//call the eventHandler (send quit as a reference)
		gui.eventHandler(hud); 
		
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
		}

		//apply all of the surfaces to surface_screen
		gui.displayAll(hud);

		//Update the screen by flipping surface_screen
		if( !gui.flipScreen() )
			return 1;

		//regulate the frame rate, and update the currentFrameRate (needs to stay at end of game loop)
		gui.frameRate(hud);
    }
    //***********************************************************************************
	//*********** End of the game loop **************************************************
	//***********************************************************************************

    //Free the images and quit SDL
    gui.clean_up(hud);

    return 0;
}