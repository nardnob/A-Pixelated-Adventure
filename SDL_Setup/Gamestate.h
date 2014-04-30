#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Ability.h"
#include "Animation.h"
#include "Entity.h"
#include "GUI.h"
#include "MenuObject.h"
#include "NPC.h"
#include "Player.h"
#include "TerrainMap.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include <vector>
using namespace std;

class GUI;

class Gamestate
{
private:
public:
	Gamestate();

	TTF_Font* font_Gamestate_1;
	TTF_Font* font_Gamestate_2;
	TTF_Font* font_Gamestate_3;

	int currentState; //The current state of the game to be calculated / displayed. i.e. Gameplay, menu system, etc.

	bool resetGameplay = true;

	GUI* gui;

	vector<int> backTo; //Vector of state ID's to go back to from menus.. like from options back to either a) the pause menu or b) the start menu

	TerrainMap currentMap; //the current map object
	vector<Player> vector_players; //vector to hold the Player object(s)
	vector<NPC> vector_NPCs;
	vector<Entity*> vector_entities; //this will hold a vector of all entities (for polymorphism to output all entities in one loop, regardless of specific types)

	vector<MenuObject*> vector_menuObjects; //A vector holding the menu objects, cleared and loaded for each new state.
	vector<int> vector_clickEvents;

	vector<Ability*> vector_abilities_player; //A vector to hold player abilities (attacks, buffs, etc) to be handled per physics frame
	vector<Ability*> vector_abilities_mobs; //A vector to hold monster attack abilities to be handled per physics frame

	vector<Animation*> vector_animations; //A vector to hold the animations (like aoe animations or player animations)

	//switch the current state
	void switchState(int newState);
	
	//set up the first currentMap
	void init();

	void defineFont();
};

#endif