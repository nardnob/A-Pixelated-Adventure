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
		boundaryH = 0;

	ifstream fin;
	fin.open(this->mapFileName);

	if(!fin)
		return false;
	
	fin >> numX >> numY;

	if(!numX || !numY || (numX > MAP_MAX_X) || (numY > MAP_MAX_Y))
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

	if(numBoundaries < 0)
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

	fin.close();
	return true;
}