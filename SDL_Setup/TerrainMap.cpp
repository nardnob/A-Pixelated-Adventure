#include "Gamestate.h"
#include "NPC.h"
#include "Player.h"
#include "TerrainMap.h"
#include "constants.cpp"
#include <string>
#include <fstream>
using namespace std;

TerrainMap::TerrainMap()
{

}

TerrainMap::TerrainMap(string mapFileName_in, Gamestate* gamestate)
{
	this->mapFileName = mapFileName_in;
	loadMap(gamestate);
}

bool TerrainMap::loadMap(Gamestate* gamestate)
{
	int 
		numX = 0,
		numY = 0,
		numBoundaries = -1,
		numMapDoor_boundaries = -1,
		numNPCs = -1,
		NPCClipNum = 0;

	double
		boundaryX = 0,
		boundaryY = 0,
		boundaryW = 0,
		boundaryH = 0,
		toX = 0,
		toY = 0,
		NPCPosX = 0,
		NPCPosY = 0,
		NPCBasePosX = 0,
		NPCBasePosY = 0,
		NPCBaseW = 0,
		NPCBaseH = 0;

	string toMap;

	ifstream fin;
	fin.open(this->mapFileName);

	if(!fin)
		return false;
	
	fin >> numX >> numY;

	if(!numX || !numY || (numX > MAP_MAX_X) || (numY > MAP_MAX_Y)) //check for incorrect map file format
	{
		fin.close();
		return false;
	}

	this->sizeX = numX;
	this->sizeY = numY;

	for(int yTimes = 0; yTimes < numY; yTimes++)
	{
		for(int xTimes = 0; xTimes < numX; xTimes++)
		{
			fin >> this->mapData[xTimes][yTimes];
		}
	}

	fin >> numBoundaries;

	if(numBoundaries < 0) //check for incorrect map file format
	{
		fin.close();
		return false;
	}

	for(int i = 0; i < numBoundaries; i++)
	{
		fin >> boundaryX >> boundaryY >> boundaryW >> boundaryH;
		this->boundaries.push_back( Boundary(
			boundaryX * TERRAIN_CLIP_W, 
			boundaryY * TERRAIN_CLIP_H, 
			boundaryW * TERRAIN_CLIP_W, 
			boundaryH * TERRAIN_CLIP_H, 
			true) );
	}

	fin >> numMapDoor_boundaries;

	if(numMapDoor_boundaries < 0) //check for incorrect map file format
	{
		fin.close();
		return false;
	}

	for(int i = 0; i < numMapDoor_boundaries; i++)
	{
		fin 
			>> boundaryX
			>> boundaryY
			>> boundaryW
			>> boundaryH
			>> toMap
			>> toX
			>> toY;

		this->mapDoor_boundaries.push_back( MapDoor_Boundary(
			boundaryX * TERRAIN_CLIP_W, 
			boundaryY * TERRAIN_CLIP_H, 
			boundaryW * TERRAIN_CLIP_W, 
			boundaryH * TERRAIN_CLIP_H,
			true,
			toMap,
			toX * TERRAIN_CLIP_W,
			toY * TERRAIN_CLIP_H) );
	}

	//clear the NPC vector
	//clear the entities pointer vector (while keeping the player pointer in position 0)
	//fill the NPC vector while filling pointers to the entities vector
	Entity* player = gamestate->vector_entities.at(0);
	gamestate->vector_entities.clear();
	gamestate->vector_NPCs.clear();
	gamestate->vector_entities.push_back(player);
	
	fin >> numNPCs;
	if(numNPCs < 0) //check for incorrect map file format
	{
		fin.close();
		return false;
	}

	gamestate->vector_entities.reserve(10);
	for(int i = 1; i <= numNPCs; i++)
	{
		fin
			>> NPCClipNum
			>> NPCPosX
			>> NPCPosY
			>> NPCBasePosX
			>> NPCBasePosY
			>> NPCBaseW
			>> NPCBaseH;

		//define a player by putting them into the player vector, then putting a pointer of them into the entity vector (for polymorphism output)
		gamestate->vector_NPCs.push_back(NPC(
			( (NPCClipNum * ENTITY_CLIP_W) % ENTITY_FILE_W ), //clipX, the x value of the entity clip (in the entity texture file)
			( ((NPCClipNum * ENTITY_CLIP_W) / ENTITY_FILE_W) * ENTITY_CLIP_H ), //clipY, the y value of the entity to clip (in the entity texture file)
			NPCPosX, //posX
			NPCPosY, //posY
			NPCBasePosX, //base_posX
			NPCBasePosY, //base_posY
			NPCBaseW, //base_w
			NPCBaseH  //base_h
			));

		//gamestate->vector_entities.push_back( &gamestate->vector_NPCs.back() );
	}

	for(int i = 0; i < gamestate->vector_NPCs.size(); i++)
	{
		gamestate->vector_entities.push_back( &gamestate->vector_NPCs.at(i) );
	}

	fin.close();
	return true;
}