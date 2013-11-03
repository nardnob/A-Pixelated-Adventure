#include "TerrainMap.h"
#include "constants.cpp"
#include <string>
#include <fstream>
using namespace std;

TerrainMap::TerrainMap()
{

}

TerrainMap::TerrainMap(string mapFileName_in)
{
	this->mapFileName = mapFileName_in;
	loadMap();
}

bool TerrainMap::loadMap()
{
	int 
		numX = 0,
		numY = 0,
		numBoundaries = -1,
		boundaryX = 0,
		boundaryY = 0,
		boundaryW = 0,
		boundaryH = 0,
		numMapDoor_boundaries = -1,
		toX = 0,
		toY = 0;

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

	fin.close();
	return true;
}