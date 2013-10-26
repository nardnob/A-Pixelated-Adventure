#include "TerrainMap.h"
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
	int numX = 0;
	int numY = 0;

	ifstream fin;
	fin.open(this->mapFileName);

	if(!fin)
		return false;
	
	fin >> numX >> numY;

	if(!numX || !numY || (numX > MAP_MAX_X) || (numY > MAP_MAX_Y))
		return false;

	this->sizeX = numX;
	this->sizeY = numY;

	for(int yTimes = 0; yTimes < numY; yTimes++)
	{
		for(int xTimes = 0; xTimes < numX; xTimes++)
		{
			fin >> this->mapData[xTimes][yTimes];

		}
	}

	return true;
}