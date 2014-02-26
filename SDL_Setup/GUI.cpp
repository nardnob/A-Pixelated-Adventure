#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "GUI.h"
#include "HUD.h"
#include "MapDoor_Boundary.h"
#include "Timer.h"

#include <ctime>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;

GUI::GUI()
{

}

GUI::GUI(Gamestate* in_gamestate) 
{ 
	this->gamestatePtr = in_gamestate; 
	
	this->surface_terrain = NULL;
	this->surface_entities = NULL;
	this->surface_screen = NULL;
	this->surface_messager = NULL;

	fullscreen = true;
	quit = false;

	//center the window; does not center the fullscreen window
	putenv("SDL_VIDEO_CENTERED=1");
}

void GUI::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, clip, destination, &offset );
}

void GUI::clean_up(HUD& hud)
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

void GUI::defineClip(int code_in)
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
	case CODE_PLAYER:	
		//define a player by putting them into the player vector, then putting a pointer of them into the entity vector (for polymorphism output)
		gamestatePtr->vector_players.push_back(Player(
			( (0 * ENTITY_CLIP_W) % ENTITY_FILE_W ), //clipX, the x value of the entity clip (in the entity texture file)
			( ((0 * ENTITY_CLIP_W) / ENTITY_FILE_W) * ENTITY_CLIP_H ), //clipY, the y value of the entity to clip (in the entity texture file)
			100, //posX
			100, //posY
			BASE_POSX, //base_posX
			BASE_POSY, //base_posY
			BASE_W, //base_w
			BASE_H  //base_h
			));

		gamestatePtr->vector_entities.push_back(&gamestatePtr->vector_players.at(0));
		break;
	}
}

void GUI::defineHUD(HUD& hud)
{
	//open the HUD font
	hud.font_HUD_1 = TTF_OpenFont(FONT_HUD_1_FILENAME, FONT_HUD_1_SIZE);

	hud.HUD_rect.h = HUD_HEIGHT;
	hud.HUD_rect.x = 0;
	if(this->fullscreen)
	{
		hud.HUD_rect.y = this->monitorHeight - HUD_HEIGHT;
		hud.HUD_rect.w = this->monitorWidth;
	}
	else
	{
		hud.HUD_rect.y = SCREEN_HEIGHT - HUD_HEIGHT;
		hud.HUD_rect.w = SCREEN_WIDTH;
	}

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

	string temp = "MAP: " + gamestatePtr->currentMap.get_mapFileName();
	hud.advancedMessages.at(hud.MESSAGE_CURRENTMAP).set_message(temp.c_str());
}

void GUI::display(int code_in, HUD& hud)
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

void GUI::display(int code_in)
{
	switch(code_in)
	{
	case CODE_TERRAIN:
		for(int numX = 0; numX < gamestatePtr->currentMap.get_sizeX(); numX++)
		{
			for(int numY = 0; numY < gamestatePtr->currentMap.get_sizeY(); numY++)
			{
				apply_surface( 
					numX * TERRAIN_CLIP_W + this->screenOffset_x, 
					numY * TERRAIN_CLIP_H + this->screenOffset_y, 
					surface_terrain, 
					surface_screen, 
					&terrainClip[gamestatePtr->currentMap.mapData[numX][numY]] );
			}
		}
		break;
	case CODE_ENTITY:
		for(int i = gamestatePtr->vector_entities.size() - 1; i >= 0; i--)
		{
			apply_surface( 
				gamestatePtr->vector_entities.at(i)->posX + this->screenOffset_x, 
				gamestatePtr->vector_entities.at(i)->posY + this->screenOffset_y, 
				surface_entities, 
				surface_screen, 
				&gamestatePtr->vector_entities.at(i)->rect[gamestatePtr->vector_entities.at(i)->get_currentTexture()] );
		}
		break;
	}
}

void GUI::displayAll(HUD& hud)
{
	SDL_FillRect(surface_screen , NULL , 0);

	//apply the terrain 
	display(CODE_TERRAIN);
	//apply the entities
	display(CODE_ENTITY);
	//display the HUD
	display(CODE_HUD, hud);
}

void GUI::eventHandler(HUD& hud)
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
				gamestatePtr->vector_players.at(0).pressKey(KEY_LEFT);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				gamestatePtr->vector_players.at(0).pressKey(KEY_RIGHT);
				break;
			case SDLK_UP:
			case SDLK_w:
				gamestatePtr->vector_players.at(0).pressKey(KEY_UP);
				break;
			case SDLK_DOWN:
			case SDLK_s:
				gamestatePtr->vector_players.at(0).pressKey(KEY_DOWN);
				break;
			case SDLK_F2:
				screenShot();
				OutputDebugString("screenShot() finished\n");
				break;
			case SDLK_F3:
				hud.toggleAdvanced();
				OutputDebugString("hud.toggleAdvanced() finished\n");
				break;
				/*//just testing
			case SDLK_1:
				for(int i = 0; i < gamestate.vector_NPCs.size(); i++)
				{
					gamestate.vector_NPCs.at(i).toggleTexture(0);
				}
				break;
			case SDLK_2:
				for(int i = 0; i < gamestate.vector_NPCs.size(); i++)
				{
					gamestate.vector_NPCs.at(i).toggleTexture(1);
				}
				break;
			case SDLK_3:
				for(int i = 0; i < gamestate.vector_NPCs.size(); i++)
				{
					gamestate.vector_NPCs.at(i).toggleTexture(2);
				}
				break;
			case SDLK_4:
				for(int i = 0; i < gamestate.vector_NPCs.size(); i++)
				{
					gamestate.vector_NPCs.at(i).toggleTexture(3);
				}
				break;
				*/
			case SDLK_F11:/*
				toggleFullscreen(
					fullscreen, 
					gamestatePtr->currentMap.get_sizeX() * TERRAIN_CLIP_W, 
					gamestatePtr->currentMap.get_sizeY() * TERRAIN_CLIP_H + HUD_HEIGHT,
					hud);*/
				toggleFullscreen(
					fullscreen, 
					SCREEN_WIDTH, 
					SCREEN_HEIGHT,
					hud);
				OutputDebugString("toggleFullscreen() finished\n");
				break;
			case SDLK_F12:
				toggleMap(hud);
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
				gamestatePtr->vector_players.at(0).releaseKey(KEY_LEFT);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				gamestatePtr->vector_players.at(0).releaseKey(KEY_RIGHT);
				break;
			case SDLK_UP:
			case SDLK_w:
				gamestatePtr->vector_players.at(0).releaseKey(KEY_UP);
				break;
			case SDLK_DOWN:
			case SDLK_s:
				gamestatePtr->vector_players.at(0).releaseKey(KEY_DOWN);
				break;
			}
			break;
		case SDL_QUIT:
			quit = true;
			break;
		}
    }	
}

bool GUI::flipScreen()
{
	if(SDL_Flip( surface_screen ) == -1)
		return false;
	return true;
}

void GUI::frameRate(HUD& hud)
{
	this->fpsTimer.frameCount++;
	int temp = this->fpsTimer.get_totalTicks();

	//count the frame rate every 10 frames
	if(this->fpsTimer.frameCount == 10)
	{
		this->fpsTimer.currentFrameRate = this->fpsTimer.frameCount / ((this->fpsTimer.get_totalTicks() - this->fpsTimer.firstStartTicks) / 1000);
		this->fpsTimer.frameCount = 0;
		this->fpsTimer.firstStartTicks = this->fpsTimer.get_totalTicks();
		hud.advancedMessages.at(hud.MESSAGE_FPS).set_message("FPS: " + to_string(static_cast<long long>(this->fpsTimer.currentFrameRate)));
	}


	//If we want to cap the frame rate
    if(this->fpsTimer.get_ticks() < 1000 / FRAMES_PER_SECOND)
    {
        //Sleep the remaining frame time
        SDL_Delay((1000 / FRAMES_PER_SECOND) - this->fpsTimer.get_ticks());
    }
}

bool GUI::init()
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
	//surface_screen = SDL_SetVideoMode( monitorWidth, monitorHeight, SCREEN_BPP, SDL_SWSURFACE | SDL_FULLSCREEN ); //full screen
	//this->fullscreen = true;
    //surface_screen = SDL_SetVideoMode( this->gamestatePtr->currentMap.get_sizeX() * TERRAIN_CLIP_W, this->gamestatePtr->currentMap.get_sizeY() * TERRAIN_CLIP_H + HUD_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ); //not full screen
	surface_screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
	this->fullscreen = false;

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

bool GUI::load_files()
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

SDL_Surface* GUI::load_image( std::string filename )
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

void GUI::screenShot()
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

void GUI::setScreenOffsets(HUD& hud)
{
	this->screenOffset_x = (this->surface_screen->w - (gamestatePtr->currentMap.get_sizeX() * TERRAIN_CLIP_W))/2;
	this->screenOffset_y = (this->surface_screen->h - gamestatePtr->currentMap.get_sizeY() * TERRAIN_CLIP_H - hud.HUD_rect.h)/2;
}

void GUI::setWindowIcon()
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

void GUI::switchMap(string in_mapFileName, double in_x, double in_y, HUD& hud)
{
	gamestatePtr->currentMap = TerrainMap(in_mapFileName, gamestatePtr);

	string temp = "MAP: " + gamestatePtr->currentMap.get_mapFileName();
	teleport(in_x, in_y);
	/*
	//defineHUD(currentMap.get_sizeX() * TERRAIN_CLIP_W, currentMap.get_sizeY() * TERRAIN_CLIP_H, currentMap, hud);
	hud.HUD_rect.w = currentMap.get_sizeX() * TERRAIN_CLIP_W;
	hud.HUD_rect.h = HUD_HEIGHT;
	hud.HUD_rect.x = 0;
	hud.HUD_rect.y = currentMap.get_sizeY() * TERRAIN_CLIP_H;*/
	temp = "MAP: " + gamestatePtr->currentMap.get_mapFileName();

	hud.advancedMessages.at(hud.MESSAGE_CURRENTMAP).set_message(temp.c_str());

	//need to set new boundary at this x and y to not ...? Not sure what this comment means, lol

	setScreenOffsets(hud);
	/*
	if(!fullscreen)
	{
		setVideoMode2(false, SCREEN_WIDTH, SCREEN_HEIGHT);

		setScreenOffsets(hud);
	}*/
}

void GUI::teleport(double in_x, double in_y, int in_direction)
{
	gamestatePtr->vector_players.at(0).posX = in_x;
	gamestatePtr->vector_players.at(0).posY = in_y;

	if(in_direction >= 0)
		gamestatePtr->vector_players.at(0).set_currentTexture(in_direction);
}

void GUI::toggleFullscreen(bool& fullscreen, int screenWidth, int screenHeight, HUD& hud)
{
	Uint32 flags; /* Start with whatever flags you prefer */

	flags = surface_screen->flags; /* Save the current flags in case toggling fails */

	if(fullscreen)
	{
		surface_screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ); //windowed mode
		//this->setVideoMode2(fullscreen, screenWidth, screenHeight);
		fullscreen = false;
		setWindowIcon(); //re-set the windows icon
		hud.HUD_rect.w = SCREEN_WIDTH;
	} 
	else
	{
		surface_screen = SDL_SetVideoMode( monitorWidth, monitorHeight, SCREEN_BPP, SDL_SWSURFACE | SDL_FULLSCREEN ); //full screen
		//this->setVideoMode2(fullscreen, monitorWidth, monitorHeight);
		fullscreen = true;
		hud.HUD_rect.w = this->monitorWidth;
	}

	if(this->fullscreen)
		hud.HUD_rect.y = this->monitorHeight - HUD_HEIGHT;
	else
		hud.HUD_rect.y = SCREEN_HEIGHT - HUD_HEIGHT;

	/* If toggle FullScreen failed, then switch back */
	if(surface_screen == NULL) 
		surface_screen = SDL_SetVideoMode(screenWidth, screenHeight, SCREEN_BPP, flags); 

	 /* If you can't switch back for some reason */
	if(surface_screen == NULL) 
		exit(1);

	setScreenOffsets(hud);
}

void GUI::toggleMap(HUD& hud)
{	
	if(gamestatePtr->currentMap.get_mapFileName() == "map_001.txt")
	{
		this->switchMap("map_002.txt", 512, 0, hud);
	}
	else
	{
		this->switchMap("map_001.txt", 512, 768, hud);
	}
}