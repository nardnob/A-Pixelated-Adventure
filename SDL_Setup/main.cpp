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
#include "Physics.h"

#include <Windows.h> //for OutputDebugString()
#include <string>
#include <vector>
using namespace std;



int main( int argc, char* args[] )
{	
	Gamestate gamestate = Gamestate();	

	GUI gui = GUI(&gamestate);

	gui.defineClip(CODE_ENTITY, gamestate);

	HUD hud = HUD(); //the HUD (holds the hud surface(s) and messages

	//define the clips (clip up the texture files)
	gui.defineClip(CODE_TERRAIN, gamestate);
	
	//set up the first currentMap
	gamestate.init();

    bool quit = false; //to quit the main game loop

	//center the window; does not center the fullscreen window
	putenv("SDL_VIDEO_CENTERED=1");

	//SDL's init()
	if( !gui.init(
		gamestate.currentMap.get_sizeX() * TERRAIN_CLIP_W, 
		gamestate.currentMap.get_sizeY() * TERRAIN_CLIP_H + HUD_HEIGHT
		) )
        return 1;

	//Initialize SDL_ttf
    if( TTF_Init() == -1 )
        return false;    

	//to define the HUD and its messages
	gui.defineHUD(gamestate.currentMap.get_sizeX() * TERRAIN_CLIP_W, gamestate.currentMap.get_sizeY() * TERRAIN_CLIP_H, gamestate.currentMap, hud);


	//SDL load_files to load images
    if( !gui.load_files() )
        return 1;

    //***********************************************************************************
	//*********** The game loop *********************************************************
	//***********************************************************************************
    while( !quit )
    {
		gui.fpsTimer.start();

		//call the eventHandler (send quit as a reference)
		gui.eventHandler(quit, hud, gamestate.currentMap.get_sizeX() * TERRAIN_CLIP_W, gamestate.currentMap.get_sizeY() * TERRAIN_CLIP_H + HUD_HEIGHT, gamestate.currentMap, gamestate); 
		
		//do some physics
		Physics::doPhysics(gamestate, hud, gui);

		//apply all of the surfaces to surface_screen
		gui.displayAll(gamestate.currentMap, hud, gamestate);

		//Update the screen by flipping surface_screen
		if( !gui.flipScreen() )
			return 1;

		//regulate the frame rate, and update the currentFrameRate
		gui.frameRate(hud);
    }
    //***********************************************************************************
	//*********** End of the game loop **************************************************
	//***********************************************************************************

    //Free the images and quit SDL
    gui.clean_up(hud);

    return 0;
}