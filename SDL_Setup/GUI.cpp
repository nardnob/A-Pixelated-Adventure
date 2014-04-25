#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "GUI.h"
#include "HUD.h"
#include "MapDoor_Boundary.h"
#include "Physics.h"
#include "Timer.h"

#include <algorithm>
#include <ctime>
#include <cmath>
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
	this->surface_healthbar = NULL;
	this->surface_buttons = NULL;
	this->surface_titleBar = NULL;
	this->surface_credits = NULL;
	this->surface_cursor_primary = NULL;

	fullscreen = false;
	quit = false;

	//center the window; does not center the fullscreen window
	putenv("SDL_VIDEO_CENTERED=1");
}

void GUI::apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}

void GUI::clean_up()
{
	//The screen surface unallocates automatically

	//Free the sprite map
	SDL_FreeSurface(surface_terrain);
	SDL_FreeSurface(surface_entities);
	SDL_FreeSurface(surface_healthbar);
	SDL_FreeSurface(surface_buttons);
	SDL_FreeSurface(surface_titleBar);
	SDL_FreeSurface(surface_credits);
	SDL_FreeSurface(surface_cursor_primary);

	//Close the font that was used
	TTF_CloseFont(hudPtr->font_HUD_1);
	TTF_CloseFont(gamestatePtr->font_Gamestate_1);
	TTF_CloseFont(gamestatePtr->font_Gamestate_2);
	TTF_CloseFont(gamestatePtr->font_Gamestate_3);

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
				terrainClip[i].x = (i * TERRAIN_CLIP_W) % TERRAIN_FILE_W;
				terrainClip[i].y = ((i * TERRAIN_CLIP_W) / TERRAIN_FILE_W) * TERRAIN_CLIP_H;
				terrainClip[i].w = TERRAIN_CLIP_W;
				terrainClip[i].h = TERRAIN_CLIP_H;
			}
			break;
		case CODE_PLAYER:
			//define a player by putting them into the player vector, then putting a pointer of them into the entity vector (for polymorphism output)
			gamestatePtr->vector_players.push_back(Player(
				0 * ENTITY_CLIP_W, //clipX, the x value of the entity clip (in the entity texture file)
				ENTITY_CLIP_H, //clipY, the y value of the entity to clip (in the entity texture file)
				100, //posX
				100, //posY
				BASE_POSX, //base_posX
				BASE_POSY, //base_posY
				BASE_W, //base_w
				BASE_H //base_h
				));

			gamestatePtr->vector_entities.push_back(&gamestatePtr->vector_players.at(0));
			break;
		case CODE_UI:
			if(this->smallMonitor)
			{
				//Title bar "A Pixelated Adventure"
				uiClip[0].w = 752;
				uiClip[0].h = 290;
				uiClip[0].x = 0;
				uiClip[0].y = 0;

				//generic title bar for menus
				uiClip[1].w = 413;
				uiClip[1].h = 244;
				uiClip[1].x = 752;
				uiClip[1].y = 0;

				button_1_width = 460;
				button_1_height = 55;
			}
			else
			{
				//Title bar "A Pixelated Adventure"
				uiClip[0].w = 1056;
				uiClip[0].h = 408;
				uiClip[0].x = 0;
				uiClip[0].y = 0;

				//generic title bar for menus
				uiClip[1].w = 582;
				uiClip[1].h = 342;
				uiClip[1].x = 1056;
				uiClip[1].y = 0;

				button_1_width = 647;
				button_1_height = 78;
			}
			break;
	}
}

void putpixel(SDL_Surface* screen, int x, int y)
{
	Uint32 *pixel = (Uint32*)screen->pixels;
	Uint32 *p = pixel + y*screen->pitch / 4 + x;
	*p = SDL_MapRGB(screen->format, 0x00, 0x64, 0x00);
}

void DrawCircle(SDL_Surface* screen, int radius)
{
	for(int i = 0; i <= 360; i++)
	{
		putpixel(screen, radius * cos(i), radius * sin(i));
	}
}

void GUI::defineHUD()
{
	//open the HUD font
	hudPtr->font_HUD_1 = TTF_OpenFont(FONT_HUD_1_FILENAME, FONT_HUD_1_SIZE);

	hudPtr->HUD_rect.h = HUD_HEIGHT;
	hudPtr->HUD_rect.x = 0;
//	if(this->fullscreen)
//	{
	hudPtr->HUD_rect.y = this->monitorHeight - HUD_HEIGHT;
	hudPtr->HUD_rect.w = this->monitorWidth;
//	}
	/*else
	{
		hudPtr->HUD_rect.y = SCREEN_HEIGHT - HUD_HEIGHT;
		hudPtr->HUD_rect.w = SCREEN_WIDTH;
	}*/

	hudPtr->healthBar_BG.w = HEALTHBAR_WIDTH;
	hudPtr->healthBar_BG.h = HEALTHBAR_HEIGHT;
	hudPtr->healthBar_BG.x = hudPtr->HUD_rect.x + HEALTHBAR_OFFSET_X;
	hudPtr->healthBar_BG.y = hudPtr->HUD_rect.y + hudPtr->HUD_rect.h / 2 - hudPtr->healthBar_BG.h / 2 + HEALTHBAR_OFFSET_Y; //center it (vertically) in the HUD_rect

	hudPtr->healthBar.w = HEALTHBAR_WIDTH;
	hudPtr->healthBar.h = HEALTHBAR_HEIGHT;
	hudPtr->healthBar.x = hudPtr->HUD_rect.x + HEALTHBAR_OFFSET_X;
	hudPtr->healthBar.y = hudPtr->HUD_rect.y + hudPtr->HUD_rect.h / 2 - hudPtr->healthBar.h / 2 + HEALTHBAR_OFFSET_Y; //center it (vertially) in the HUD_rect
	
	//healthbar messagec
	hudPtr->hudMessages.push_back(Message(
		hudPtr->healthBar.x + hudPtr->healthBar.w / 2 + HEALTHBAR_MESSAGE_OFFSET_X,
		hudPtr->healthBar.y + hudPtr->healthBar.h / 2 + HEALTHBAR_MESSAGE_OFFSET_Y,
		hudPtr->font_HUD_1,
		"100 / 100",
		true
		));

	//advanced messages
	hudPtr->advancedMessages.push_back(Message(
		20,
		20 * 1,
		hudPtr->font_HUD_1,
		"Vel X: ",
		true));

	hudPtr->advancedMessages.push_back(Message(
		20,
		20 * 2,
		hudPtr->font_HUD_1,
		"Vel Y: ",
		true));

	hudPtr->advancedMessages.push_back(Message(
		20,
		20 * 3,
		hudPtr->font_HUD_1,
		"Pos X: ",
		true));

	hudPtr->advancedMessages.push_back(Message(
		20,
		20 * 4,
		hudPtr->font_HUD_1,
		"Pos Y: ",
		true));

	hudPtr->advancedMessages.push_back(Message(
		20,
		20 * 5,
		hudPtr->font_HUD_1,
		"FPS:  ",
		true));

	hudPtr->advancedMessages.push_back(Message(
		20,
		20 * 6,
		hudPtr->font_HUD_1,
		"MAP:  ",
		true));

	hudPtr->advancedMessages.push_back(Message(
		20,
		20 * 7,
		hudPtr->font_HUD_1,
		"Life:  ",
		true));
	
	string temp = "MAP: " + gamestatePtr->currentMap.get_mapFileName();
	hudPtr->advancedMessages.at(hudPtr->MESSAGE_CURRENTMAP).set_message(temp.c_str());
}

bool sortEntitiesFunc(Entity* i, Entity* j)
{
	return ((i->posY + i->base_posY) > (j->posY + j->base_posY));
}

void GUI::display(int code_in)
{
	switch(code_in)
	{
		//display the hud:
		//	display HUD rect at bottom of screen
		//	loop through advanced messages and display them
		case CODE_HUD:
			
			//Display all entity healthbars above them
			for(int i = 0; i < gamestatePtr->vector_entities.size(); i++)
			{
				gamestatePtr->vector_entities.at(i)->healthBar.x += this->screenOffset_x;
				gamestatePtr->vector_entities.at(i)->healthBar.y += this->screenOffset_y;
				gamestatePtr->vector_entities.at(i)->healthBar_BG.x += this->screenOffset_x;
				gamestatePtr->vector_entities.at(i)->healthBar_BG.y += this->screenOffset_y;
				gamestatePtr->vector_entities.at(i)->healthBar_border.x += this->screenOffset_x;
				gamestatePtr->vector_entities.at(i)->healthBar_border.y += this->screenOffset_y;
				/*
				SDL_Rect temp_healthBar = gamestatePtr->vector_entities.at(i)->healthBar;
				SDL_Rect temp_healthBar_BG = gamestatePtr->vector_entities.at(i)->healthBar_BG;
				SDL_Rect temp_healthBar_border = gamestatePtr->vector_entities.at(i)->healthBar_border;
				temp_healthBar.x += this->screenOffset_x;
				temp_healthBar.y += this->screenOffset_y;
				temp_healthBar_BG.x += this->screenOffset_x;
				temp_healthBar_BG.y += this->screenOffset_y;
				temp_healthBar_border.x += this->screenOffset_x;
				temp_healthBar_border.y += this->screenOffset_y;
				*/

				//if(
				//	gamestatePtr->vector_entities.at(i)->healthBar_border.x >= screenOffset_x
				//	&& gamestatePtr->vector_entities.at(i)->healthBar_border.y >= screenOffset_y
				//	&& gamestatePtr->vector_entities.at(i)->healthBar_border.x + gamestatePtr->vector_entities.at(i)->healthBar_border.w <= screenOffset_x + gamestatePtr->currentMap.get_sizeX() * TERRAIN_CLIP_W
				//	&& gamestatePtr->vector_entities.at(i)->healthBar_border.y + gamestatePtr->vector_entities.at(i)->healthBar_border.h <= screenOffset_y + gamestatePtr->currentMap.get_sizeY() * TERRAIN_CLIP_H
				//	) //make sure the healthbars don't blit offscreen (or they'll mess up I think)
				if(
					gamestatePtr->vector_entities.at(i)->healthBar_border.x >= 0
					&& gamestatePtr->vector_entities.at(i)->healthBar_border.y >= 0
					&& gamestatePtr->vector_entities.at(i)->healthBar_border.x + gamestatePtr->vector_entities.at(i)->healthBar_border.w < monitorWidth
					&& gamestatePtr->vector_entities.at(i)->healthBar_border.y + gamestatePtr->vector_entities.at(i)->healthBar_border.h < monitorHeight
					)
				{
					//display the entitie's gray healthbar border
					SDL_FillRect(surface_screen, &gamestatePtr->vector_entities.at(i)->healthBar_border, SDL_MapRGB(surface_screen->format, 86, 86, 86));

					//display the entitie's red healthbar
					SDL_FillRect(surface_screen, &gamestatePtr->vector_entities.at(i)->healthBar_BG, SDL_MapRGB(surface_screen->format, 185, 0, 0));

					//update the width of the health bars foreground (the green) and display it
					gamestatePtr->vector_entities.at(i)->healthBar.w = HEALTHBAR_ENTITY_WIDTH * gamestatePtr->vector_entities.at(i)->currentStatus.lifePercent();
					SDL_FillRect(surface_screen, &gamestatePtr->vector_entities.at(i)->healthBar, SDL_MapRGB(surface_screen->format, 0, 185, 0));
				}

				gamestatePtr->vector_entities.at(i)->healthBar.x -= this->screenOffset_x;
				gamestatePtr->vector_entities.at(i)->healthBar.y -= this->screenOffset_y;
				gamestatePtr->vector_entities.at(i)->healthBar_BG.x -= this->screenOffset_x;
				gamestatePtr->vector_entities.at(i)->healthBar_BG.y -= this->screenOffset_y;
				gamestatePtr->vector_entities.at(i)->healthBar_border.x -= this->screenOffset_x;
				gamestatePtr->vector_entities.at(i)->healthBar_border.y -= this->screenOffset_y;
			}

			//fill in the background of the HUD with gray (86, 86, 86 RGB)
			SDL_FillRect(surface_screen, &hudPtr->HUD_rect, SDL_MapRGB(surface_screen->format, 86, 86, 86));

			//fill in the health bar background with red
			SDL_FillRect(surface_screen, &hudPtr->healthBar_BG, SDL_MapRGB(surface_screen->format, 185, 0, 0));

			//update the width of the health bars foreground (the green) and display it
			hudPtr->healthBar.w = HEALTHBAR_WIDTH * gamestatePtr->vector_players.at(0).currentStatus.lifePercent();
			SDL_FillRect(surface_screen, &hudPtr->healthBar, SDL_MapRGB(surface_screen->format, 0, 185, 0));

			//display the overlay of the healthbar
			apply_surface(hudPtr->healthBar.x - 75, hudPtr->healthBar.y - 12, surface_healthbar, surface_screen);

			//display the hud messages
			for(int i = 0; i < hudPtr->hudMessages.size(); i++)
			{
				hudPtr->hudMessages.at(i).display(this->surface_messager, this->surface_buttons, this->surface_screen);
			}

			//display the advanced messages if advanced is enabled in hud. (if f3 was pressed)
			for(int i = 0; i < hudPtr->advancedMessages.size(); i++)
			{
				if(hudPtr->get_advanced()) //if displaying advanced messages
				{
					hudPtr->advancedMessages.at(i).display(this->surface_messager, this->surface_buttons, this->surface_screen);
				}
			}
			break;		

		//loop through the terrain and output each clip
		case CODE_TERRAIN:
				for(int numX = 0; numX < gamestatePtr->currentMap.get_sizeX(); numX++)
				{
					for(int numY = 0; numY < gamestatePtr->currentMap.get_sizeY(); numY++)
					{
						//this if is for debugging.. to prevent blitting off screen
						//if(numX * TERRAIN_CLIP_W + this->screenOffset_x + TERRAIN_CLIP_W <= SCREEN_WIDTH && numY * TERRAIN_CLIP_H + this->screenOffset_y + TERRAIN_CLIP_H <= SCREEN_HEIGHT)
						apply_surface(
							numX * TERRAIN_CLIP_W + this->screenOffset_x,
							numY * TERRAIN_CLIP_H + this->screenOffset_y,
							surface_terrain,
							surface_screen,
							&terrainClip[gamestatePtr->currentMap.mapData[numX][numY]]);
					}
				}
				break;

			//loop through and display the entities (a vector of good guys and bad guys and every entity ever)
			case CODE_ENTITY:
				//sort the Entities by depth and then output them
				vector<Entity*> tempEntities;
				for(int i = 0; i < gamestatePtr->vector_entities.size(); i++)
				{
					tempEntities.push_back(gamestatePtr->vector_entities.at(i));
				}

				//sort the entities by depth, lowest posY first
				sort(tempEntities.begin(), tempEntities.end(), sortEntitiesFunc);
				
				for(int i = tempEntities.size() - 1; i >= 0; i--)
				{
					SDL_Rect temp = tempEntities.at(i)->rect[tempEntities.at(i)->get_currentTexture()];
					temp.y += tempEntities.at(i)->spriteOffsetY;

					apply_surface(
						tempEntities.at(i)->posX + this->screenOffset_x,
						tempEntities.at(i)->posY + this->screenOffset_y,
						surface_entities,
						surface_screen,
						&temp);
						//&gamestatePtr->vector_entities.at(i)->rect[gamestatePtr->vector_entities.at(i)->get_currentTexture()]);
				}
				break;
	}
}

void GUI::displayAll()
{
	SDL_FillRect(surface_screen, NULL, 0);

	/*
	Here are the options for displaying

	//apply the terrain
	display(CODE_TERRAIN, hud);

	//apply the entities
	display(CODE_ENTITY, hud);

	//display the HUD
	display(CODE_HUD, hud);
	*/
	
	//OutputDebugString(("currentState: " + to_string(gamestatePtr->currentState)).c_str());
	switch(gamestatePtr->currentState)
	{
		case STATES_GAMEPLAY:
			SDL_FillRect(surface_screen, NULL, SDL_MapRGB(surface_screen->format, 0, 0, 0));
			display(CODE_TERRAIN);
			display(CODE_ENTITY);
			display(CODE_HUD);
			break;

		case STATES_START_MENU:
			SDL_FillRect(surface_screen, NULL, SDL_MapRGB(surface_screen->format, 60, 52, 43));

			apply_surface(this->monitorWidth / 2 - uiClip[UI_CLIP_TITLE].w / 2, 0, surface_titleBar, surface_screen, &this->uiClip[UI_CLIP_TITLE]);

			//Display all of the MenuObjects
			for(int i = 0; i < gamestatePtr->vector_menuObjects.size(); i++)
			{
				gamestatePtr->vector_menuObjects.at(i)->display(surface_messager, surface_buttons, surface_screen);
			}
			break;

		case STATES_PAUSE_MENU:
			SDL_FillRect(surface_screen, NULL, SDL_MapRGB(surface_screen->format, 60, 52, 43));

			//Display all of the MenuObjects
			for(int i = 0; i < gamestatePtr->vector_menuObjects.size(); i++)
			{
				gamestatePtr->vector_menuObjects.at(i)->display(this->surface_messager, this->surface_buttons, this->surface_screen);
			}
			break;

		case STATES_DEATH_MENU:
			SDL_FillRect(surface_screen, NULL, SDL_MapRGB(surface_screen->format, 60, 52, 43));
			//Display all of the MenuObjects
			for(int i = 0; i < gamestatePtr->vector_menuObjects.size(); i++)
			{
				gamestatePtr->vector_menuObjects.at(i)->display(this->surface_messager, this->surface_buttons, this->surface_screen);
			}
			break;

		case STATES_OPTIONS_MENU:
			SDL_FillRect(surface_screen, NULL, SDL_MapRGB(surface_screen->format, 60, 52, 43));

			apply_surface(this->monitorWidth / 2 - uiClip[UI_CLIP_HEADER].w / 2, 0, surface_titleBar, surface_screen, &this->uiClip[UI_CLIP_HEADER]);

			//Display all of the MenuObjects
			for(int i = 0; i < gamestatePtr->vector_menuObjects.size(); i++)
			{
				gamestatePtr->vector_menuObjects.at(i)->display(this->surface_messager, this->surface_buttons, this->surface_screen);
			}
			break;

		case STATES_CREDITS_MENU:
			SDL_FillRect(surface_screen, NULL, SDL_MapRGB(surface_screen->format, 60, 52, 43));

			apply_surface(this->monitorWidth / 2 - uiClip[UI_CLIP_HEADER].w / 2, 0, surface_titleBar, surface_screen, &this->uiClip[UI_CLIP_HEADER]);

			apply_surface(this->monitorWidth / 2 - this->surface_credits->w / 2, this->monitorHeight / 10 + uiClip[UI_CLIP_HEADER].h, surface_credits, surface_screen);

			//Display all of the MenuObjects
			for(int i = 0; i < gamestatePtr->vector_menuObjects.size(); i++)
			{
				gamestatePtr->vector_menuObjects.at(i)->display(this->surface_messager, this->surface_buttons, this->surface_screen);
			}
			break;

	}

	//apply the custom cursor to the screen
	apply_surface(mouse_x, mouse_y, surface_cursor_primary, surface_screen);
}

void GUI::handleMouseMotion(int x, int y)
{
	mouse_x = x;
	mouse_y = y;

	for(int i = 0; i < gamestatePtr->vector_menuObjects.size(); i++)
	{
		gamestatePtr->vector_menuObjects.at(i)->handleMouseOver(x, y);
	}
}

void GUI::handleMouseDown(int x, int y)
{
	for(int i = 0; i < gamestatePtr->vector_menuObjects.size(); i++)
	{
		gamestatePtr->vector_menuObjects.at(i)->handleMouseDown(x, y);
	}
}

void GUI::handleMouseUp(int x, int y)
{
	for(int i = 0; i < gamestatePtr->vector_menuObjects.size(); i++)
	{
		gamestatePtr->vector_menuObjects.at(i)->handleMouseUp(x, y);
	}
}

void GUI::eventHandler()
{
	//While there's events to handle
	while(SDL_PollEvent(&event))
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
					case SDLK_SPACE:
						gamestatePtr->vector_players.at(0).pressKey(KEY_SPACE);
						break;
					case SDLK_ESCAPE:
						gamestatePtr->vector_players.at(0).pressKey(KEY_ESC);
						break;
					case SDLK_F2:
						screenShot();
						break;
					case SDLK_F3:
						hudPtr->toggleAdvanced();
						break;
					//case SDLK_F11:/*
					//	toggleFullscreen(
					//	fullscreen,
					//	gamestatePtr->currentMap.get_sizeX() * TERRAIN_CLIP_W,
					//	gamestatePtr->currentMap.get_sizeY() * TERRAIN_CLIP_H + HUD_HEIGHT,
					//	hud);*/
					//	toggleFullscreen(
					//		fullscreen,
					//		hud);
					//	OutputDebugString("toggleFullscreen() finished\n");
					case SDLK_F12:
						toggleMap();
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
					case SDLK_SPACE:
						gamestatePtr->vector_players.at(0).releaseKey(KEY_SPACE);
						break;
					case SDLK_ESCAPE:
						gamestatePtr->vector_players.at(0).releaseKey(KEY_ESC);
				}
				break;

			case SDL_MOUSEMOTION:
				handleMouseMotion(event.button.x, event.button.y);
				break;

			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == SDL_BUTTON_LEFT)
					handleMouseDown(event.button.x, event.button.y);
				break;

			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT)
					handleMouseUp(event.button.x, event.button.y);
				break;

			case SDL_QUIT:
				quit = true;
				break;
		}
	}
}

bool GUI::flipScreen()
{
	if(SDL_Flip(surface_screen) == -1)
		return false;
	return true;
}

void GUI::frameRate()
{
	//If the framerate is above the cap and physics is good to go
	if(this->fpsTimer.get_ticks() >= 1000 / FRAMES_PER_SECOND)
	{
		//Sleep the remaining frame time
		//SDL_Delay((1000 / FRAMES_PER_SECOND) - this->fpsTimer.get_ticks());
		this->fpsTimer.allowPhysics = true;
		OutputDebugString("\n\n1\n\n");

		this->fpsTimer.frameCount++;
		int temp = this->fpsTimer.get_totalTicks();

		//count the frame rate every 10 frames
		if(this->fpsTimer.frameCount == 10)
		{
			this->fpsTimer.currentFrameRate = this->fpsTimer.frameCount / ((this->fpsTimer.get_totalTicks() - this->fpsTimer.firstStartTicks) / 1000);
			this->fpsTimer.frameCount = 0;
			this->fpsTimer.firstStartTicks = this->fpsTimer.get_totalTicks();
			hudPtr->advancedMessages.at(hudPtr->MESSAGE_FPS).set_message("FPS: " + to_string(static_cast<long long>(this->fpsTimer.currentFrameRate)));
		}
	}
	else
	{
		this->fpsTimer.allowPhysics = false;
		OutputDebugString("\n\n2\n\n");
	}
}

bool GUI::init()
{
	//Initialize all SDL subsystems
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}

	const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();   
	monitorWidth = info->current_w;
	monitorHeight = info->current_h;

	if(monitorWidth != 1920 && monitorHeight != 1080)
	{
		this->smallMonitor = true;
	}
	else
	{
		this->smallMonitor = false;
	}

	//set the window icon 32x32 bmp. 0 255 0: colorkey for transparency
	setWindowIcon();

	//Set up the screen (only enable one)
	//surface_screen = SDL_SetVideoMode( monitorWidth, monitorHeight, SCREEN_BPP, SDL_SWSURFACE | SDL_FULLSCREEN ); //full screen
	//this->fullscreen = true;
	//surface_screen = SDL_SetVideoMode( this->gamestatePtr->currentMap.get_sizeX() * TERRAIN_CLIP_W, this->gamestatePtr->currentMap.get_sizeY() * TERRAIN_CLIP_H + HUD_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ); //not full screen
	//surface_screen = SDL_SetVideoMode(monitorWidth, monitorHeight, SCREEN_BPP, SDL_SWSURFACE);
	surface_screen = SDL_SetVideoMode(monitorWidth, monitorHeight, SCREEN_BPP, SDL_SWSURFACE | SDL_FULLSCREEN); //full screen
	//surface_screen = SDL_SetVideoMode(monitorWidth, monitorHeight, SCREEN_BPP, SDL_SWSURFACE);
	this->fullscreen = true;

	//If there was an error in setting up the screen
	if(surface_screen == NULL)
	{
		return false;
	}

	//Set the window caption
	SDL_WM_SetCaption("A Pixelated Adventure", NULL);

	//hide the cursor to display the custom cursor instead
	SDL_ShowCursor(0);

	//If everything initialized fine
	return true;
}

bool GUI::load_files()
{
	//Load the sprite map
	surface_terrain = load_image("terrain.png");
	surface_entities = load_image("entities.png");
	surface_healthbar = load_image("healthbar.png");
	surface_cursor_primary = load_image("cursor_primary.png");
	if(this->smallMonitor)
	{
		surface_buttons = load_image("buttons_small.png");
		surface_titleBar = load_image("title_small.png");
		surface_credits = load_image("credits_small.png");
	}
	else
	{
		surface_buttons = load_image("buttons.png");
		surface_titleBar = load_image("title.png");
		surface_credits = load_image("credits.png");
	}

	if(surface_terrain == NULL
	   || surface_entities == NULL
	   || surface_healthbar == NULL
	   || surface_buttons == NULL
	   || surface_titleBar == NULL
	   || surface_credits == NULL
	   || surface_cursor_primary == NULL)
	{
		return false;
	}

	return true;
}

SDL_Surface* GUI::load_image(std::string filename)
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;
	loadedImage = IMG_Load(filename.c_str());

	if(loadedImage != NULL)
	{
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_FreeSurface(loadedImage);

		if(optimizedImage != NULL)
		{
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
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

void GUI::setScreenOffsets()
{
	this->screenOffset_x = (this->surface_screen->w - (gamestatePtr->currentMap.get_sizeX() * TERRAIN_CLIP_W)) / 2;
	this->screenOffset_y = (this->surface_screen->h - gamestatePtr->currentMap.get_sizeY() * TERRAIN_CLIP_H - hudPtr->HUD_rect.h) / 2;
}

void GUI::setWindowIcon()
{
	Uint32 colorKey;
	SDL_Surface *image;

	image = SDL_LoadBMP(ICON_FILE.c_str());
	colorKey = SDL_MapRGB(image->format, 0, 255, 0);
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorKey);
	SDL_WM_SetIcon(image, NULL);
	SDL_FreeSurface(image);
}

void GUI::switchMap(string in_mapFileName, double in_x, double in_y)
{
	gamestatePtr->currentMap = TerrainMap(in_mapFileName, gamestatePtr);

	string temp = "MAP: " + gamestatePtr->currentMap.get_mapFileName();
	teleport(in_x, in_y);

	//Set the map file name for advanced messages (i.e. Map: map_001.txt)
	hudPtr->advancedMessages.at(hudPtr->MESSAGE_CURRENTMAP).set_message(temp);

	setScreenOffsets();

	Physics::updateHealthbarPos(gamestatePtr, 0);
}

void GUI::teleport(double in_x, double in_y, int in_direction)
{
	gamestatePtr->vector_players.at(0).posX = in_x;
	gamestatePtr->vector_players.at(0).posY = in_y;

	if(in_direction >= 0)
		gamestatePtr->vector_players.at(0).set_currentTexture(in_direction);
}

void GUI::toggleFullscreen(bool& fullscreen)
{
	Uint32 flags; /* Start with whatever flags you prefer */

	flags = surface_screen->flags; /* Save the current flags in case toggling fails */

	if(fullscreen)
	{
		surface_screen = SDL_SetVideoMode(monitorWidth, monitorWidth, SCREEN_BPP, SDL_SWSURFACE); //windowed mode
		//this->setVideoMode2(fullscreen, screenWidth, screenHeight);
		fullscreen = false;
		setWindowIcon(); //re-set the windows icon
		hudPtr->HUD_rect.w = SCREEN_WIDTH;
		//hudPtr->HUD_rect.w = 10;
		hudPtr->advancedMessages.at(6).set_message("Fullscreen:  false");
		hudPtr->HUD_rect.y = SCREEN_HEIGHT - HUD_HEIGHT;
	}
	else
	{
		surface_screen = SDL_SetVideoMode(monitorWidth, monitorHeight, SCREEN_BPP, SDL_SWSURFACE | SDL_FULLSCREEN); //full screen
		//this->setVideoMode2(fullscreen, monitorWidth, monitorHeight);
		fullscreen = true;
		hudPtr->HUD_rect.w = this->monitorWidth;
		//hudPtr->HUD_rect.w = 10;
		hudPtr->advancedMessages.at(6).set_message("Fullscreen:  true");
		hudPtr->HUD_rect.y = this->monitorHeight - HUD_HEIGHT;
	}

	/* If toggle FullScreen failed, then switch back */
	if(surface_screen == NULL)
		surface_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, flags);

	/* If you can't switch back for some reason */
	if(surface_screen == NULL)
		exit(1);

	setScreenOffsets();
}

void GUI::toggleMap()
{
	if(gamestatePtr->currentMap.get_mapFileName() == "map_001.txt")
	{
		this->switchMap("map_002.txt", 512, 0);
	}
	else
	{
		this->switchMap("map_001.txt", 512, 768);
	}
}