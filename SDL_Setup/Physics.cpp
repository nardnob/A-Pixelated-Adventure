#include "Physics.h"
#include "Player.h"
#include "HUD.h"
#include "Boundary.h"
#include "constants.cpp"

#include <Windows.h>
#include <vector>
#include <cmath>
using namespace std;

/*
bool badPosition(double inX, double inY, vector<Boundary>& boundaries)
{
    bool
        badX = false,
        badY = false;

    for(int i = 0; i < boundaries.size(); i++)
    {
		if( inX >= boundaries.at(i).x && inX < (boundaries.at(i).x + boundaries.at(i).w) )
			badX = true;

		if( inY >= boundaries.at(i).y && inY < (boundaries.at(i).y + boundaries.at(i).h) )
			badY = true;

		if(badX && badY)
			return true;

		badX = false;
		badY = false;
	}

	return false;
}
*/

bool badPosition(double inX, double inY, vector<Boundary>& boundaries, Player& player)
{
    double
		pX1 = inX + player.base_posX, //player box left side
		pX2 = inX + player.base_posX + player.base_w, //player box right side
		pY1 = inY + player.base_posY, //player box top
		pY2 = inY + player.base_posY + player.base_h, //player box bottom
		bX1 = 0, //bound box left side
		bX2 = 0, //bound box right side
		bY1 = 0, //bound box top
		bY2 = 0; //bound box bottom

	bool badX, badY;

	for(int i = 0; i < boundaries.size(); i++)
	{
		badX = true;
		badY = true;

		bX1 = boundaries.at(i).x;
		bX2 = boundaries.at(i).x + boundaries.at(i).w;
		bY1 = boundaries.at(i).y;
		bY2 = boundaries.at(i).y + boundaries.at(i).h;

		if( (pX1 > bX2)	|| (pX2 < bX1) )
			badX = false;

		if(	(pY1 > bY2)	|| (pY2 < bY1) )
			badY = false;

		if(badX && badY)
		{
			return true;
		}
	}
	return false;
}

vector<bool> goodNextPosition(Player& player, double nextX, double nextY, vector<Boundary>& boundaries)
{
	vector<bool> bad;
	bad.push_back(false);
	bad.push_back(false);
	enum { x, y };

	if( badPosition(nextX, player.posY, boundaries, player) )
		bad.at(x) = true;

	if( badPosition(player.posX, nextY, boundaries, player) )
		bad.at(y) = true;

	return bad;
}

void resolveDoorCollisions(Player& player, vector<MapDoor_Boundary>& mapDoor_boundaries, GUI& gui, TerrainMap& currentMap, HUD& hud)
{    
	double
		pX1 = player.posX + player.base_posX, //player box left side
		pX2 = player.posX + player.base_posX + player.base_w, //player box right side
		pY1 = player.posY + player.base_posY, //player box top
		pY2 = player.posY + player.base_posY + player.base_h, //player box bottom
		bX1 = 0, //bound box left side
		bX2 = 0, //bound box right side
		bY1 = 0, //bound box top
		bY2 = 0; //bound box bottom

	bool inX, inY, done = false;

	for(int i = 0; i < mapDoor_boundaries.size() && !done; i++)
	{
		inX = true;
		inY = true;

		bX1 = mapDoor_boundaries.at(i).x;
		bX2 = mapDoor_boundaries.at(i).x + mapDoor_boundaries.at(i).w;
		bY1 = mapDoor_boundaries.at(i).y;
		bY2 = mapDoor_boundaries.at(i).y + mapDoor_boundaries.at(i).h;

		if( (pX1 > bX2)	|| (pX2 < bX1) )
			inX = false;

		if(	(pY1 > bY2)	|| (pY2 < bY1) )
			inY = false;

		if(inX && inY)
		{
			if(mapDoor_boundaries.at(i).inTheDoorway)
			{
			}
			else
			{
				gui.switchMap(mapDoor_boundaries.at(i).toMap, mapDoor_boundaries.at(i).toX, mapDoor_boundaries.at(i).toY, currentMap, hud);
				mapDoor_boundaries.at(i).inTheDoorway = true;
			}

			done = true;
		}
		else
		{
			mapDoor_boundaries.at(i).inTheDoorway = false;
		}
	}
	
}

void Physics::doPhysics(vector<Player>& inVector, HUD& hud, vector<Boundary>& boundaries, vector<MapDoor_Boundary>& mapDoor_boundaries, GUI& gui, TerrainMap& currentMap)
{	
	//consider keyboard events for the player
	for(int i = 0; i < 4; i++)
	{
		if(inVector.at(0).keyIsDown(i))
		{
			switch(i)
			{
			case KEY_LEFT:
				inVector.at(0).forceX = inVector.at(0).forceX - WALKING_FORCE;
				inVector.at(0).toggleTexture(TEXTURE_LEFT);
				break;
			case KEY_RIGHT:
				inVector.at(0).forceX += WALKING_FORCE;
				inVector.at(0).toggleTexture(TEXTURE_RIGHT);
				break;
			case KEY_UP:
				inVector.at(0).forceY = inVector.at(0).forceY - WALKING_FORCE;
				inVector.at(0).toggleTexture(TEXTURE_BACK);
				break;
			case KEY_DOWN:
				inVector.at(0).forceY += WALKING_FORCE;
				inVector.at(0).toggleTexture(TEXTURE_FRONT);
				break;
			}
		}
	}
	
	//convert force to velocity
	for(int i = 0; i < inVector.size(); i++)
	{
		if(inVector.at(i).mass != 0)
		{
			inVector.at(i).velX = inVector.at(i).velX + inVector.at(i).forceX / inVector.at(i).mass; //Vf = Vi + F/m (Vf = Vi + at => Vf = Vi + F/m*1)
			inVector.at(i).velY = inVector.at(i).velY + inVector.at(i).forceY / inVector.at(i).mass;

			if(i == 0) //only want to update the HUD for the player
			{
				if(inVector.at(i).velX >= 0) //the if is just to keep the velocity's from jumping over one when it's negative.. for looks. Unnecessary performance hit here
					hud.advancedMessages.at(hud.MESSAGE_VELX).set_message("Vel X:  " + to_string(int(inVector.at(i).velX)));
				else
					hud.advancedMessages.at(hud.MESSAGE_VELX).set_message("Vel X: " + to_string(int(inVector.at(i).velX)));

				if(inVector.at(i).velY >= 0)
					hud.advancedMessages.at(hud.MESSAGE_VELY).set_message("Vel Y:  " + to_string(int(inVector.at(i).velY)));
				else
					hud.advancedMessages.at(hud.MESSAGE_VELY).set_message("Vel Y: " + to_string(int(inVector.at(i).velY)));
			}
		}
		inVector.at(i).forceX = 0;
		inVector.at(i).forceY = 0;
	}

	//limit velocity
	for(int i = 0; i < inVector.size(); i++)
	{
		if(inVector.at(i).velX > inVector.at(i).maxVelX)
			inVector.at(i).velX = inVector.at(i).maxVelX;
		else
			if(inVector.at(i).velX < -inVector.at(i).maxVelX)
				inVector.at(i).velX = -inVector.at(i).maxVelX;
		if(inVector.at(i).velY > inVector.at(i).maxVelY)
			inVector.at(i).velY = inVector.at(i).maxVelY;
		else
			if(inVector.at(i).velY < -inVector.at(i).maxVelY)
				inVector.at(i).velY = -inVector.at(i).maxVelY;
	}

	//calculate collisions

	//resolve collisions

	//calculate movement (need to limit position to bounds of window)
	for(int i = 0; i < inVector.size(); i++)
	{
		//test for boundary collisions; only update position if no boundary collision with said update
		double
			nextX = inVector.at(i).posX + inVector.at(i).velX,
			nextY = inVector.at(i).posY + inVector.at(i).velY;

		vector<bool> bad = goodNextPosition(inVector.at(i), nextX, nextY, boundaries);
		enum { x, y };

		if( !bad.at(x) )
		{
			inVector.at(i).posX = nextX;
		}
		if( !bad.at(y) )
		{
			inVector.at(i).posY = nextY;
		}
		
		if(i == 0) //only want to update the HUD for the player
		{
			if(inVector.at(i).posX >= 0) //only for appearance.. can remove this and the next if
				hud.advancedMessages.at(hud.MESSAGE_POSX).set_message("Pos X:  " + to_string(int(inVector.at(i).posX)));
			else			
				hud.advancedMessages.at(hud.MESSAGE_POSX).set_message("Pos X: " + to_string(int(inVector.at(i).posX)));

			if(inVector.at(i).posY >= 0)
				hud.advancedMessages.at(hud.MESSAGE_POSY).set_message("Pos Y:  " + to_string(int(inVector.at(i).posY)));
			else			
				hud.advancedMessages.at(hud.MESSAGE_POSY).set_message("Pos Y: " + to_string(int(inVector.at(i).posY)));
		}
	}

	//check if MapDoor_Boundary was entered, if so load a new map
	resolveDoorCollisions(inVector.at(0), mapDoor_boundaries, gui, currentMap, hud);

	//calculate friction //currently bugged and working on
	for(int i = 0; i < inVector.size(); i++)
	{
		//calculating friction for the x axis
		if(abs(inVector.at(i).velX) < STATIC_FRICTION) //if it's going too slow to beat static friction (to avoid oscillation)
		//if(sqrt(pow(inVector.at(i).velX, 2) + pow(inVector.at(i).velY, 2)) < STATIC_FRICTION) 
		{
			inVector.at(i).velX = 0;
			//inVector.at(i).velY = 0;//for the vector math
		}
		else
		{
			if(inVector.at(i).velX > 0)
			{
				inVector.at(i).forceX = inVector.at(i).forceX - FRICTION;
			} else
				if(inVector.at(i).velX < 0)
				{
					inVector.at(i).forceX += FRICTION;
				}
		}

		//calculating friction for the y axis
		if(abs(inVector.at(i).velY) < STATIC_FRICTION) //if it's going too slow to beat static friction (to avoid oscillation)		
		//if(sqrt(pow(inVector.at(i).velX, 2) + pow(inVector.at(i).velY, 2)) < STATIC_FRICTION) //vector math instead of component-based
		{
			inVector.at(i).velY = 0;
		}
		else
		{
			if(inVector.at(i).velY > 0)
			{
				inVector.at(i).forceY = inVector.at(i).forceY - FRICTION;
			} else
				if(inVector.at(i).velY < 0)
				{
					inVector.at(i).forceY += FRICTION;
				}
		}
	}
}