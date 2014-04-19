#ifndef GUI_H
#define GUI_H

#include "constants.cpp"
#include "Gamestate.h"
#include "HUD.h"
#include "TerrainMap.h"
#include "Timer.h"

#include <string>
using namespace std;

class Gamestate;

class GUI
{
private:
	Gamestate* gamestatePtr;	

	//The event structure
	SDL_Event event;

	//The portions of the sprite map to be blitted
	SDL_Rect terrainClip[ TERRAIN_CLIP_COUNT ];
	bool fullscreen;	//the window state (full screen or windowed)
public:
	//The surfaces
	SDL_Surface* surface_terrain;
	SDL_Surface* surface_entities;
	SDL_Surface* surface_screen;
	SDL_Surface* surface_messager;
	SDL_Surface* surface_titleBar;
	SDL_Surface* surface_healthbar;
	SDL_Surface* surface_buttons;

	int monitorWidth, monitorHeight; //the width and height of the monitor defined at runtime

	int mouse_x, mouse_y;

	Timer fpsTimer; //timer to regulate and monitor the frames per second
	bool quit; //only true when done with the game

	double screenOffset_x, screenOffset_y; //the offset to display the screen at, to place it in the middle of the window

	HUD* hudPtr;

	GUI();
	GUI(Gamestate* in_gamestate);

	//apply a surface to another surface
	void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );

	//destroy SDL to exit the program
	void clean_up();

	//define the clips
	void defineClip(int code_in);

	//define the HUD
	void defineHUD();

	////display the _ to the screen surface
	//void display(int code_in);

	//display the _ to the screen surface
	void display(int code_in);
	
	//display all of the surfaces to the screen surface
	void displayAll();

	//the games event handler to process keyboard input and mouse input
	void eventHandler();

	//flip the current screen to the buffer
	bool flipScreen();

	//limit the framerate and update the current framerate to display
	void frameRate();

	void handleMouseMotion(int x, int y);

	void handleMouseDown(int x, int y);

	void handleMouseUp(int x, int y);

	//initialize SDL
	bool init();

	//load files into SDL, like images
	bool load_files();

	//load an image
	SDL_Surface *load_image(string filename);

	//take a screenshot
	void screenShot();

	//set the offsets of the surface_screen
	void setScreenOffsets();

	//set the window icon to a constant image file
	void setWindowIcon();

	//switches from one map to another
	void switchMap(string in_mapFileName, double in_x, double in_y);

	//teleports the player 0 to (x, y) in the in_direction
	void teleport(double in_x, double in_y, int in_direction = -1);

	//toggle between fullscreen and windowed mode
	void toggleFullscreen(bool& fullscreen);

	//toggle between map_001.txt and map_002.txt; used for testing
	void toggleMap();
};

#endif