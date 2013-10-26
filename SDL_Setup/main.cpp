/*
The SDL setup functions used in this code were created by following the lazyfoo tutorials at lazyfoo.net
*/

//The headers
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "constants.cpp"
#include "Entity.h"
#include "HUD.h"
#include "Physics.h"
#include "Player.h"
#include "TerrainMap.h"
#include "Timer.h"

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
vector<Player> vector_players; //vector to hold the Player object(s)


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

void setWindowIcon(char in_file[])
{
	Uint32 colorKey;
	SDL_Surface *image;

	image = SDL_LoadBMP(in_file);
	colorKey = SDL_MapRGB(image->format, 0, 255, 0);
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorKey);
	SDL_WM_SetIcon(image, NULL);
	SDL_FreeSurface(image);
}

bool init(int screenWidth, int screenHeight)
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }
	
	//set the window icon 32x32 bmp. 0 255 0: colorkey for transparency
	setWindowIcon( "icon2.bmp" );

    //Set up the screen (only enable one)
    //surface_screen = SDL_SetVideoMode( screenWidth, screenHeight, SCREEN_BPP, SDL_FULLSCREEN ); //full screen
    surface_screen = SDL_SetVideoMode( screenWidth, screenHeight, SCREEN_BPP, SDL_SWSURFACE ); //not full screen

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

void defineClip(int code_in)
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
		vector_players.push_back(Player(
			( (0 * ENTITY_CLIP_W) % ENTITY_FILE_W ), //the x value of the entity clip (in the entity texture file)
			( ((0 * ENTITY_CLIP_W) / ENTITY_FILE_W) * ENTITY_CLIP_H ) //the y value of the entity to clip (in the entity texture file)
			));

		vector_entities.push_back(&vector_players[0]);
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
		

}

void toggleFullscreen(bool& fullscreen, int screenWidth, int screenHeight)
{
	if(fullscreen)
	{
		surface_screen = SDL_SetVideoMode( screenWidth, screenHeight, SCREEN_BPP, SDL_SWSURFACE ); //not full screen
		fullscreen = false;
	} 
	else
	{
		surface_screen = SDL_SetVideoMode( screenWidth, screenHeight, SCREEN_BPP, SDL_FULLSCREEN ); //full screen
		fullscreen = true;
	}
}

void screenShot()
{
	time_t t = time(NULL);
	tm* timePtr = localtime(&t);

	string fileName = "screenshot " + to_string(timePtr->tm_mon) + "_" + to_string(timePtr->tm_mday) + "_" + to_string(timePtr->tm_year + 1900) + "_";
	int attempt = 0;

	//while the filename already exists, add another number to the file name
	for(; std::ifstream(fileName + to_string(attempt) + ".bmp"); attempt++)
	{		
	}

	fileName = fileName + to_string(attempt) + ".bmp";
	const char* temp = fileName.c_str();
	SDL_SaveBMP( surface_screen,temp); 
}

void eventHandler(bool& quit, HUD& hud, bool& fullscreen, int screenWidth, int screenHeight)
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
				vector_players.at(0).pressKey(KEY_LEFT);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				vector_players.at(0).pressKey(KEY_RIGHT);
				break;
			case SDLK_UP:
			case SDLK_w:
				vector_players.at(0).pressKey(KEY_UP);
				break;
			case SDLK_DOWN:
			case SDLK_s:
				vector_players.at(0).pressKey(KEY_DOWN);
				break;
			case SDLK_F3:
				hud.toggleAdvanced();
				break;
			case SDLK_F11:
				toggleFullscreen(fullscreen, screenWidth, screenHeight);
				break;
			case SDLK_F12:
				screenShot();
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
				vector_players.at(0).releaseKey(KEY_LEFT);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				vector_players.at(0).releaseKey(KEY_RIGHT);
				break;
			case SDLK_UP:
			case SDLK_w:
				vector_players.at(0).releaseKey(KEY_UP);
				break;
			case SDLK_DOWN:
			case SDLK_s:
				vector_players.at(0).releaseKey(KEY_DOWN);
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

int main( int argc, char* args[] )
{	
	//initialize the currentMap object with the mapFileName text file
	TerrainMap currentMap = TerrainMap("map_001.txt");
	Timer fpsTimer;
	HUD hud = HUD();

	//define the clips (clip up the texture files)
	defineClip(CODE_TERRAIN);
	defineClip(CODE_ENTITY);

	//to quit the main game loop
    bool quit = false;

	//just playing around. alrighty, guess it worked. Hopefully no memory leaks created from it
	bool fullscreen = false;

	//SDL's init()
	if( !init(currentMap.get_sizeX() * TERRAIN_CLIP_W, currentMap.get_sizeY() * TERRAIN_CLIP_H + HUD_HEIGHT) )
        return 1;
	
	//Initialize SDL_ttf
    if( TTF_Init() == -1 )
        return false;    

	//to define the HUD and its messages
	defineHUD(currentMap.get_sizeX() * TERRAIN_CLIP_W, currentMap.get_sizeY() * TERRAIN_CLIP_H, currentMap, hud);

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
		eventHandler(quit, hud, fullscreen, currentMap.get_sizeX() * TERRAIN_CLIP_W, currentMap.get_sizeY() * TERRAIN_CLIP_H + HUD_HEIGHT); 

		//do some physics
		Physics::doPhysics(vector_players, hud);
	
		//apply the terrain 
		display(CODE_TERRAIN, currentMap);
		//apply the entities
		display(CODE_ENTITY, currentMap);
		//display the HUD
		display(CODE_HUD, currentMap, hud);
		
		//Update the screen
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