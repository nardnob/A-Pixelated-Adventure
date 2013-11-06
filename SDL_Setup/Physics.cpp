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

void keyboardInput(Gamestate& gamestate)
{
	for(int i = 0; i < 4; i++)
	{
		if(gamestate.vector_players.at(0).keyIsDown(i))
		{
			switch(i)
			{
			case KEY_LEFT:
				gamestate.vector_players.at(0).forceX = gamestate.vector_players.at(0).forceX - WALKING_FORCE;
				gamestate.vector_players.at(0).toggleTexture(TEXTURE_LEFT);
				break;
			case KEY_RIGHT:
				gamestate.vector_players.at(0).forceX += WALKING_FORCE;
				gamestate.vector_players.at(0).toggleTexture(TEXTURE_RIGHT);
				break;
			case KEY_UP:
				gamestate.vector_players.at(0).forceY = gamestate.vector_players.at(0).forceY - WALKING_FORCE;
				gamestate.vector_players.at(0).toggleTexture(TEXTURE_BACK);
				break;
			case KEY_DOWN:
				gamestate.vector_players.at(0).forceY += WALKING_FORCE;
				gamestate.vector_players.at(0).toggleTexture(TEXTURE_FRONT);
				break;
			}
		}
	}
}

void resolveDoorCollisions(Gamestate& gamestate, GUI& gui, HUD& hud)
{    
	double
		pX1 = gamestate.vector_players.at(0).posX + gamestate.vector_players.at(0).base_posX, //gamestate.vector_players.at(0) box left side
		pX2 = gamestate.vector_players.at(0).posX + gamestate.vector_players.at(0).base_posX + gamestate.vector_players.at(0).base_w, //gamestate.vector_players.at(0) box right side
		pY1 = gamestate.vector_players.at(0).posY + gamestate.vector_players.at(0).base_posY, //gamestate.vector_players.at(0) box top
		pY2 = gamestate.vector_players.at(0).posY + gamestate.vector_players.at(0).base_posY + gamestate.vector_players.at(0).base_h, //player box bottom
		bX1 = 0, //bound box left side
		bX2 = 0, //bound box right side
		bY1 = 0, //bound box top
		bY2 = 0; //bound box bottom

	bool inX, inY, done = false;

	for(int i = 0; (i < gamestate.currentMap.mapDoor_boundaries.size()) && !done; i++)
	{
		inX = true;
		inY = true;

		bX1 = gamestate.currentMap.mapDoor_boundaries.at(i).x;
		bX2 = gamestate.currentMap.mapDoor_boundaries.at(i).x + gamestate.currentMap.mapDoor_boundaries.at(i).w;
		bY1 = gamestate.currentMap.mapDoor_boundaries.at(i).y;
		bY2 = gamestate.currentMap.mapDoor_boundaries.at(i).y + gamestate.currentMap.mapDoor_boundaries.at(i).h;

		if( (pX1 > bX2)	|| (pX2 < bX1) )
			inX = false;

		if(	(pY1 > bY2)	|| (pY2 < bY1) )
			inY = false;

		if(inX && inY)
		{
			if(gamestate.currentMap.mapDoor_boundaries.at(i).inTheDoorway)
			{
			}
			else
			{
				gui.switchMap(gamestate.currentMap.mapDoor_boundaries.at(i).toMap, gamestate.currentMap.mapDoor_boundaries.at(i).toX, gamestate.currentMap.mapDoor_boundaries.at(i).toY, hud);
			}

			done = true;
		}
		else
		{
			gamestate.currentMap.mapDoor_boundaries.at(i).inTheDoorway = false;
		}
	}
	
}

void Physics::doPhysics(Gamestate& gamestate, HUD& hud, GUI& gui)
{	
	//consider keyboard events for the player
	keyboardInput(gamestate);
	
	//convert force to velocity
	for(int i = 0; i < gamestate.vector_players.size(); i++)
	{
		if(gamestate.vector_players.at(i).mass != 0)
		{
			gamestate.vector_players.at(i).velX = gamestate.vector_players.at(i).velX + gamestate.vector_players.at(i).forceX / gamestate.vector_players.at(i).mass; //Vf = Vi + F/m (Vf = Vi + at => Vf = Vi + F/m*1)
			gamestate.vector_players.at(i).velY = gamestate.vector_players.at(i).velY + gamestate.vector_players.at(i).forceY / gamestate.vector_players.at(i).mass;

			if(i == 0) //only want to update the HUD for the player
			{
				if(gamestate.vector_players.at(i).velX >= 0) //the if is just to keep the velocity's from jumping over one when it's negative.. for looks. Unnecessary performance hit here
					hud.advancedMessages.at(hud.MESSAGE_VELX).set_message("Vel X:  " + to_string(int(gamestate.vector_players.at(i).velX)));
				else
					hud.advancedMessages.at(hud.MESSAGE_VELX).set_message("Vel X: " + to_string(int(gamestate.vector_players.at(i).velX)));

				if(gamestate.vector_players.at(i).velY >= 0)
					hud.advancedMessages.at(hud.MESSAGE_VELY).set_message("Vel Y:  " + to_string(int(gamestate.vector_players.at(i).velY)));
				else
					hud.advancedMessages.at(hud.MESSAGE_VELY).set_message("Vel Y: " + to_string(int(gamestate.vector_players.at(i).velY)));
			}
		}
		gamestate.vector_players.at(i).forceX = 0;
		gamestate.vector_players.at(i).forceY = 0;
	}

	//limit velocity
	for(int i = 0; i < gamestate.vector_players.size(); i++)
	{
		if(gamestate.vector_players.at(i).velX > gamestate.vector_players.at(i).maxVelX)
			gamestate.vector_players.at(i).velX = gamestate.vector_players.at(i).maxVelX;
		else
			if(gamestate.vector_players.at(i).velX < -gamestate.vector_players.at(i).maxVelX)
				gamestate.vector_players.at(i).velX = -gamestate.vector_players.at(i).maxVelX;
		if(gamestate.vector_players.at(i).velY > gamestate.vector_players.at(i).maxVelY)
			gamestate.vector_players.at(i).velY = gamestate.vector_players.at(i).maxVelY;
		else
			if(gamestate.vector_players.at(i).velY < -gamestate.vector_players.at(i).maxVelY)
				gamestate.vector_players.at(i).velY = -gamestate.vector_players.at(i).maxVelY;
	}

	//calculate collisions

	//resolve collisions

	//calculate movement (need to limit position to bounds of window)
	for(int i = 0; i < gamestate.vector_players.size(); i++)
	{
		//test for boundary collisions; only update position if no boundary collision with said update
		double
			nextX = gamestate.vector_players.at(i).posX + gamestate.vector_players.at(i).velX,
			nextY = gamestate.vector_players.at(i).posY + gamestate.vector_players.at(i).velY;

		vector<bool> bad = goodNextPosition(gamestate.vector_players.at(i), nextX, nextY, gamestate.currentMap.boundaries);
		enum { x, y };

		if( !bad.at(x) )
		{
			gamestate.vector_players.at(i).posX = nextX;
		}
		if( !bad.at(y) )
		{
			gamestate.vector_players.at(i).posY = nextY;
		}
		
		if(i == 0) //only want to update the HUD for the player
		{
			if(gamestate.vector_players.at(i).posX >= 0) //only for appearance.. can remove this and the next if
				hud.advancedMessages.at(hud.MESSAGE_POSX).set_message("Pos X:  " + to_string(int(gamestate.vector_players.at(i).posX)));
			else			
				hud.advancedMessages.at(hud.MESSAGE_POSX).set_message("Pos X: " + to_string(int(gamestate.vector_players.at(i).posX)));

			if(gamestate.vector_players.at(i).posY >= 0)
				hud.advancedMessages.at(hud.MESSAGE_POSY).set_message("Pos Y:  " + to_string(int(gamestate.vector_players.at(i).posY)));
			else			
				hud.advancedMessages.at(hud.MESSAGE_POSY).set_message("Pos Y: " + to_string(int(gamestate.vector_players.at(i).posY)));
		}
	}

	//check if MapDoor_Boundary was entered, if so load a new map
	resolveDoorCollisions(gamestate, gui, hud);

	//calculate friction //currently bugged and working on
	for(int i = 0; i < gamestate.vector_players.size(); i++)
	{
		//calculating friction for the x axis
		if(abs(gamestate.vector_players.at(i).velX) < STATIC_FRICTION) //if it's going too slow to beat static friction (to avoid oscillation)
		//if(sqrt(pow(gamestate.vector_players.at(i).velX, 2) + pow(gamestate.vector_players.at(i).velY, 2)) < STATIC_FRICTION) 
		{
			gamestate.vector_players.at(i).velX = 0;
			//gamestate.vector_players.at(i).velY = 0;//for the vector math
		}
		else
		{
			if(gamestate.vector_players.at(i).velX > 0)
			{
				gamestate.vector_players.at(i).forceX = gamestate.vector_players.at(i).forceX - FRICTION;
			} else
				if(gamestate.vector_players.at(i).velX < 0)
				{
					gamestate.vector_players.at(i).forceX += FRICTION;
				}
		}

		//calculating friction for the y axis
		if(abs(gamestate.vector_players.at(i).velY) < STATIC_FRICTION) //if it's going too slow to beat static friction (to avoid oscillation)		
		//if(sqrt(pow(gamestate.vector_players.at(i).velX, 2) + pow(gamestate.vector_players.at(i).velY, 2)) < STATIC_FRICTION) //vector math instead of component-based
		{
			gamestate.vector_players.at(i).velY = 0;
		}
		else
		{
			if(gamestate.vector_players.at(i).velY > 0)
			{
				gamestate.vector_players.at(i).forceY = gamestate.vector_players.at(i).forceY - FRICTION;
			} else
				if(gamestate.vector_players.at(i).velY < 0)
				{
					gamestate.vector_players.at(i).forceY += FRICTION;
				}
		}
	}
}