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
#include "Player.h"
#include "TerrainMap.h"
#include "Timer.h"

#include <Windows.h> //for OutputDebugString()
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
using namespace std;

//The surfaces
SDL_Surface* surface_terrain = NULL;
SDL_Surface* surface_entities = NULL;
SDL_Surface* surface_screen = NULL;
SDL_Surface* surface_messager = NULL;

//The event structure
SDL_Event event;

//The portions of the sprite map to be blitted
SDL_Rect terrainClip[ TERRAIN_CLIP_COUNT ];

//SDL_Rect entityClip[ ENTITY_CLIP_COUNT ]; //used a vector to hold Player objects and stored the clips in Player instead
vector<Entity*> vector_entities; //this will hold a vector of all entities (for polymorphism to output all entities in one loop, regardless of specific types)

int monitorWidth, monitorHeight;

SDL_Surface *load_image( std::string filename )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        SDL_FreeSurface( loadedImage );

        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, clip, destination, &offset );
}

void setWindowIcon()
{
	Uint32 colorKey;
	SDL_Surface *image;

	//image = SDL_LoadBMP(ICON_FILE.c_str());
	image = SDL_LoadBMP(ICON_FILE.c_str());
	colorKey = SDL_MapRGB(image->format, 0, 255, 0);
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorKey);
	SDL_WM_SetIcon(image, NULL);
	SDL_FreeSurface(image);
}

bool init(int screenWidth, int screenHeight, bool& fullscreen)
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

	const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();   
	monitorWidth = info->current_w;
	monitorHeight = info->current_h;

	//set the window icon 32x32 bmp. 0 255 0: colorkey for transparency
	setWindowIcon();

    //Set up the screen (only enable one)
    surface_screen = SDL_SetVideoMode( monitorWidth, monitorHeight, SCREEN_BPP, SDL_SWSURFACE | SDL_FULLSCREEN ); //full screen
    //surface_screen = SDL_SetVideoMode( screenWidth, screenHeight, SCREEN_BPP, SDL_SWSURFACE ); //not full screen
	fullscreen = true;

    //If there was an error in setting up the screen
    if( surface_screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "A Pixelated Adventure", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the sprite map
    surface_terrain = load_image( "terrain.png" );
	surface_entities = load_image( "entities.png" );

    if( surface_terrain == NULL 
		|| surface_entities == NULL)
    {
        return false;
    }

    return true;
}

void clean_up(HUD& hud)
{
	//The screen surface unallocates automatically
	
    //Free the sprite map
    SDL_FreeSurface( surface_terrain );
	SDL_FreeSurface( surface_entities );
    
    //Close the font that was used
    TTF_CloseFont( hud.font_HUD_1 );
    
    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

void display(int code_in, TerrainMap& currentMap, HUD& hud)
{
	switch(code_in)
	{
	case CODE_HUD:
		//fill in the background of the HUD with gray (86, 86, 86 RGB)
		SDL_FillRect( surface_screen, &hud.HUD_rect, SDL_MapRGB( surface_screen->format, 86, 86, 86 ) );

		//display the advanced messages if advanced is enabled in hud. (if f3 was pressed)
		for(int i = 0; i < hud.advancedMessages.size(); i++)
		{
			if(i >= 0 && i < hud.advancedMessages.size() && hud.get_advanced()) //the first 0 to NUM_ADVANCED_SETTINGS are all advanced settings. They are only outputted if we are showing advanced settings. Need to store these in their own vector
			{
				surface_messager = TTF_RenderText_Solid( hud.advancedMessages.at(i).get_font(), hud.advancedMessages.at(i).get_message(), FONT_COLOR_WHITE );

				apply_surface(
					hud.advancedMessages.at(i).get_posX(),
					hud.advancedMessages.at(i).get_posY(),
					surface_messager,
					surface_screen);

				SDL_FreeSurface(surface_messager);
			}
		}
		break;
	}
}

void display(int code_in, TerrainMap& currentMap)
{
	switch(code_in)
	{
	case CODE_TERRAIN:
		for(int numX = 0; numX < currentMap.get_sizeX(); numX++)
		{
			for(int numY = 0; numY < currentMap.get_sizeY(); numY++)
			{
				apply_surface( 
					numX * 32, 
					numY * 32, 
					surface_terrain, 
					surface_screen, 
					&terrainClip[currentMap.mapData[numX][numY]] );
			}
		}
		break;
	case CODE_ENTITY:
		for(int i = 0; i < vector_entities.size(); i++)
		{
			apply_surface( 
				vector_entities.at(i)->posX, 
				vector_entities.at(i)->posY, 
				surface_entities, 
				surface_screen, 
				&vector_entities.at(i)->rect[vector_entities.at(i)->get_currentTexture()] );
		}
		break;
	}
}

void defineClip(int code_in, Gamestate& gamestate)
{
	switch(code_in)
	{
	case CODE_TERRAIN:
		//define the terrainClip array to the corresponding position of each terrain in the terrain text file
		for(int i = 0; i < TERRAIN_CLIP_COUNT; i++)
		{
			terrainClip[ i ].x = (i * TERRAIN_CLIP_W) % TERRAIN_FILE_W;
			terrainClip[ i ].y = ((i * TERRAIN_CLIP_W) / TERRAIN_FILE_W) * TERRAIN_CLIP_H;
			terrainClip[ i ].w = TERRAIN_CLIP_W;
			terrainClip[ i ].h = TERRAIN_CLIP_H;
		}
		break;
	case CODE_ENTITY:	
		//define a player by putting them into the player vector, then putting a pointer of them into the entity vector (for polymorphism output)
		gamestate.vector_players.push_back(Player(
			( (0 * ENTITY_CLIP_W) % ENTITY_FILE_W ), //clipX, the x value of the entity clip (in the entity texture file)
			( ((0 * ENTITY_CLIP_W) / ENTITY_FILE_W) * ENTITY_CLIP_H ), //clipY, the y value of the entity to clip (in the entity texture file)
			100, //posX
			100, //posY
			17, //base_posX
			41, //base_posY
			30, //base_w
			23  //base_h
			));

		vector_entities.push_back(&gamestate.vector_players.at(0));
		break;
	}
}

void defineHUD(int screen_w, int screen_h, TerrainMap& currentMap, HUD& hud)
{
	//open the HUD font
	hud.font_HUD_1 = TTF_OpenFont(FONT_HUD_1_FILENAME, FONT_HUD_1_SIZE);

	hud.HUD_rect.w = screen_w;
	hud.HUD_rect.h = HUD_HEIGHT;
	hud.HUD_rect.x = 0;
	hud.HUD_rect.y = screen_h;

	hud.advancedMessages.push_back( Message(
		20, 
		20 * 1,
		hud.font_HUD_1,
		"Vel X: ",
		true) );	
	
	hud.advancedMessages.push_back( Message(
		20, 
		20 * 2,
		hud.font_HUD_1,
		"Vel Y: ",
		true) );	

	hud.advancedMessages.push_back( Message(
		20, 
		20 * 3,
		hud.font_HUD_1,
		"Pos X: ",
		true) );	

	hud.advancedMessages.push_back( Message(
		20, 
		20 * 4,
		hud.font_HUD_1,
		"Pos Y: ",
		true) );

	hud.advancedMessages.push_back( Message(
		20, 
		20 * 5,
		hud.font_HUD_1,
		"FPS:  ",
		true) );

	hud.advancedMessages.push_back( Message(
		20, 
		20 * 6,
		hud.font_HUD_1,
		"MAP:  ",
		true) );
}

void toggleFullscreen(bool& fullscreen, int screenWidth, int screenHeight)
{
	Uint32 flags; /* Start with whatever flags you prefer */

	flags = surface_screen->flags; /* Save the current flags in case toggling fails */

	if(fullscreen)
	{
		surface_screen = SDL_SetVideoMode( screenWidth, screenHeight, SCREEN_BPP, SDL_SWSURFACE ); //windowed mode
		fullscreen = false;
		setWindowIcon(); //re-set the windows icon
	} 
	else
	{
		surface_screen = SDL_SetVideoMode( monitorWidth, monitorHeight, SCREEN_BPP, SDL_SWSURFACE | SDL_FULLSCREEN ); //full screen
		fullscreen = true;
	}

	/* If toggle FullScreen failed, then switch back */
	if(surface_screen == NULL) 
		surface_screen = SDL_SetVideoMode(screenWidth, screenHeight, SCREEN_BPP, flags); 

	 /* If you can't switch back for some reason */
	if(surface_screen == NULL) 
		exit(1);
}

void screenShot()
{
	time_t t = time(NULL);
	tm* timePtr = localtime(&t);

	//"screenshot 6_18_1992_" (if it is june 18, 1992)
	string fileName = "screenshot " + to_string(timePtr->tm_mon) + "_" + to_string(timePtr->tm_mday) + "_" + to_string(timePtr->tm_year + 1900) + "_";
	int attempt = 0;

	//while the filename already exists, add another number to the file name
	for(; std::ifstream(fileName + to_string(attempt) + ".bmp"); attempt++)
	{		
	}

	//"screenshot 6_18_1992_0.bmp" (if attempt is 0)
	fileName = fileName + to_string(attempt) + ".bmp";

	//convert the string to a const char* for the function call
	const char* temp = fileName.c_str();
	SDL_SaveBMP(surface_screen, temp);
}

//just used for testing. Alternates between map_001.txt and map_002.txt
void toggleMap(TerrainMap& currentMap, HUD& hud, GUI& gui)
{	
	if(currentMap.get_mapFileName() == "map_001.txt")
	{
		gui.switchMap("map_002.txt", 512, 0, currentMap, hud);
	}
	else
	{
		gui.switchMap("map_001.txt", 512, 768, currentMap, hud);
	}
}

void eventHandler(bool& quit, HUD& hud, bool& fullscreen, int screenWidth, int screenHeight, TerrainMap& currentMap, GUI& gui, Gamestate& gamestate)
{
	//While there's events to handle
    while( SDL_PollEvent( &event ) )
    {
		switch(event.type)
		{
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_a:
				gamestate.vector_players.at(0).pressKey(KEY_LEFT);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				gamestate.vector_players.at(0).pressKey(KEY_RIGHT);
				break;
			case SDLK_UP:
			case SDLK_w:
				gamestate.vector_players.at(0).pressKey(KEY_UP);
				break;
			case SDLK_DOWN:
			case SDLK_s:
				gamestate.vector_players.at(0).pressKey(KEY_DOWN);
				break;
			case SDLK_F2:
				screenShot();
				OutputDebugString("screenShot() finished\n");
				break;
			case SDLK_F3:
				hud.toggleAdvanced();
				OutputDebugString("hud.toggleAdvanced() finished\n");
				break;
			case SDLK_F11:
				toggleFullscreen(fullscreen, screenWidth, screenHeight);
				OutputDebugString("toggleFullscreen() finished\n");
				break;
			case SDLK_F12:
				toggleMap(currentMap, hud, gui);
				OutputDebugString("switchMap() finished\n");
				break;
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
			break;
		case SDL_KEYUP:
			switch(event.key.keysym.sym)
			{
			case SDLK_LEFT:
			case SDLK_a:
				gamestate.vector_players.at(0).releaseKey(KEY_LEFT);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				gamestate.vector_players.at(0).releaseKey(KEY_RIGHT);
				break;
			case SDLK_UP:
			case SDLK_w:
				gamestate.vector_players.at(0).releaseKey(KEY_UP);
				break;
			case SDLK_DOWN:
			case SDLK_s:
				gamestate.vector_players.at(0).releaseKey(KEY_DOWN);
				break;
			}
			break;
		case SDL_QUIT:
			quit = true;
			break;
		}
    }	
}

void frameRate(Timer& fpsTimer, HUD& hud)
{
	fpsTimer.frameCount++;
	int temp = fpsTimer.get_totalTicks();

	//count the frame rate every 10 frames
	if(fpsTimer.frameCount == 10)
	{
		fpsTimer.currentFrameRate = fpsTimer.frameCount / ((fpsTimer.get_totalTicks() - fpsTimer.firstStartTicks) / 1000);
		fpsTimer.frameCount = 0;
		fpsTimer.firstStartTicks = fpsTimer.get_totalTicks();
		hud.advancedMessages.at(hud.MESSAGE_FPS).set_message("FPS: " + to_string(fpsTimer.currentFrameRate));
	}


	//If we want to cap the frame rate
    if(fpsTimer.get_ticks() < 1000 / FRAMES_PER_SECOND)
    {
        //Sleep the remaining frame time
        SDL_Delay((1000 / FRAMES_PER_SECOND) - fpsTimer.get_ticks());
    }
}

void displayAll(TerrainMap& currentMap, HUD& hud)
{
	SDL_FillRect(surface_screen , NULL , 0);

	//apply the terrain 
	display(CODE_TERRAIN, currentMap);
	//apply the entities
	display(CODE_ENTITY, currentMap);
	//display the HUD
	display(CODE_HUD, currentMap, hud);
}

int main( int argc, char* args[] )
{	
	//initialize the currentMap object with the mapFileName text file
	TerrainMap currentMap = TerrainMap("map_001.txt");

	Gamestate gamestate = Gamestate();

	GUI gui = GUI(&gamestate);

	Timer fpsTimer; //timer to regulate and monitor the frames per second
	HUD hud = HUD(); //the HUD (holds the hud surface(s) and messages

	bool fullscreen = true;	//the window state (full screen or windowed)

	//define the clips (clip up the texture files)
	defineClip(CODE_TERRAIN, gamestate);
	defineClip(CODE_ENTITY, gamestate);

    bool quit = false; //to quit the main game loop

	//center the window; does not center the fullscreen window
	putenv("SDL_VIDEO_CENTERED=1");

	//SDL's init()
	if( !init(
		currentMap.get_sizeX() * TERRAIN_CLIP_W, 
		currentMap.get_sizeY() * TERRAIN_CLIP_H + HUD_HEIGHT, 
		fullscreen) )
        return 1;
	
	//Initialize SDL_ttf
    if( TTF_Init() == -1 )
        return false;    

	//to define the HUD and its messages
	defineHUD(currentMap.get_sizeX() * TERRAIN_CLIP_W, currentMap.get_sizeY() * TERRAIN_CLIP_H, currentMap, hud);
	string temp = "MAP: " + currentMap.get_mapFileName();
	hud.advancedMessages.at(hud.MESSAGE_CURRENTMAP).set_message(temp.c_str());

	//SDL load_files to load images
    if( !load_files() )
        return 1;

    //***********************************************************************************
	//*********** The game loop *********************************************************
	//***********************************************************************************
    while( !quit )
    {
		fpsTimer.start();

		//call the eventHandler (send quit as a reference)
		eventHandler(quit, hud, fullscreen, currentMap.get_sizeX() * TERRAIN_CLIP_W, currentMap.get_sizeY() * TERRAIN_CLIP_H + HUD_HEIGHT, currentMap, gui, gamestate); 

		//do some physics
		Physics::doPhysics(gamestate.vector_players, hud, currentMap.boundaries);
	
		//apply all of the surfaces to surface_screen
		displayAll(currentMap, hud);
		
		//Update the screen by flipping surface_screen
		if( SDL_Flip( surface_screen ) == -1 )
			return 1;

		//regulate the frame rate, and update the currentFrameRate
		frameRate(fpsTimer, hud);
    }
    //***********************************************************************************
	//*********** End of the game loop **************************************************
	//***********************************************************************************

    //Free the images and quit SDL
    clean_up(hud);

    return 0;
}