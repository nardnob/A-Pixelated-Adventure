#ifndef TERRAINMAP_H
#define TERRAINMAP_H

#include <string>
using namespace std;

const int MAP_MAX_X = 60;
const int MAP_MAX_Y = 33;

class TerrainMap
{
private:
	string mapFileName; //stored to later be able to fetch the file name to calculate physics
	int sizeX, sizeY;
public:
	TerrainMap();
	TerrainMap(string mapFileName_in);
	bool loadMap();
	int mapData[MAP_MAX_X][MAP_MAX_Y]; //screw dynamicism

	string get_mapFileName() { return mapFileName; }
	void set_mapFileName(string mapFileName_in) { this->mapFileName = mapFileName_in; }

	int get_sizeX() { return sizeX; }
	void set_sizeX(int sizeX_in) { this->sizeX = sizeX_in; }

	int get_sizeY() { return sizeY; }
	void set_sizeY(int sizeY_in) { this->sizeY = sizeY_in; }
};

#endif