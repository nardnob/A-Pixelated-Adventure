#include "Boundary.h"
#include "constants.cpp"
#include "HUD.h"
#include "Physics.h"
#include "Player.h"

#include <Windows.h>
#include <vector>
#include <cmath>
#include <math.h>
using namespace std;

#define PI 3.14159265

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

bool badPosition(double inX, double inY, vector<Boundary>& boundaries, Entity* entity, vector<Entity*>& entityVector)
{
    double
		pX1 = inX + entity->base_posX, //entity box left side
		pX2 = inX + entity->base_posX + entity->base_w, //entity box right side
		pY1 = inY + entity->base_posY, //entity box top
		pY2 = inY + entity->base_posY + entity->base_h, //entity box bottom
		bX1 = 0, //bound box left side
		bX2 = 0, //bound box right side
		bY1 = 0, //bound box top
		bY2 = 0; //bound box bottom

	bool badX, badY;

	//check boundary collisions
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

		if(badX && badY) //collision between entity and boundary
		{
			return true;
		}
	}

	//check entity collisions
	for(int i = 0; i < entityVector.size(); i++)
	{
		if(entity != entityVector.at(i))
		{
			badX = true;
			badY = true;

			bX1 = entityVector.at(i)->posX + entityVector.at(i)->base_posX;
			bX2 = bX1 + entityVector.at(i)->base_w;
			bY1 = entityVector.at(i)->posY + entityVector.at(i)->base_posY;
			bY2 = bY1 + entityVector.at(i)->base_h;

			if( (pX1 > bX2)	|| (pX2 < bX1) )
				badX = false;

			if(	(pY1 > bY2)	|| (pY2 < bY1) )
				badY = false;

			if(badX && badY) //collision between two entities
			{
				return true;
			}
		}
	}

	return false;
}

vector<bool> goodNextPosition(Gamestate& gamestate, double nextX, double nextY, Entity* entity)
{
	vector<bool> bad;
	bad.push_back(false);
	bad.push_back(false);
	enum { x, y };

	if( badPosition(nextX, entity->posY, gamestate.currentMap.boundaries, entity, gamestate.vector_entities) )
		bad.at(x) = true;

	if( badPosition(entity->posX, nextY, gamestate.currentMap.boundaries, entity, gamestate.vector_entities) )
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

void toggleNPCTextures(Gamestate& gamestate)
{
	for(int i = 0; i < gamestate.vector_NPCs.size(); i++)
	{
		if(abs(gamestate.vector_NPCs.at(i).velX) > abs(gamestate.vector_NPCs.at(i).velY))
		{
			if(gamestate.vector_NPCs.at(i).velX > 0)
			{
				gamestate.vector_NPCs.at(i).toggleTexture(TEXTURE_RIGHT);
			}
			else
			{
				gamestate.vector_NPCs.at(i).toggleTexture(TEXTURE_LEFT);
			}
		}
		else
		{
			if(gamestate.vector_NPCs.at(i).velY > 0)
			{
				gamestate.vector_NPCs.at(i).toggleTexture(TEXTURE_FRONT);
			}
			else
			{
				gamestate.vector_NPCs.at(i).toggleTexture(TEXTURE_BACK);
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

void friction(Gamestate& gamestate)
{
	//calculate friction
	for(int i = 0; i < gamestate.vector_entities.size(); i++)
	{
		//calculating friction for the x axis
		if(abs(gamestate.vector_entities.at(i)->velX) < STATIC_FRICTION) //if it's going too slow to beat static friction (to avoid oscillation)
		//if(sqrt(pow(gamestate.vector_entities.at(i)->velX, 2) + pow(gamestate.vector_entities.at(i)->velY, 2)) < STATIC_FRICTION) 
		{
			gamestate.vector_entities.at(i)->velX = 0;
			//gamestate.vector_entities.at(i)->velY = 0;//for the vector math
		}
		else
		{
			if(gamestate.vector_entities.at(i)->velX > 0)
			{
				gamestate.vector_entities.at(i)->forceX = gamestate.vector_entities.at(i)->forceX - FRICTION;
			} else
				if(gamestate.vector_entities.at(i)->velX < 0)
				{
					gamestate.vector_entities.at(i)->forceX += FRICTION;
				}
		}

		//calculating friction for the y axis
		if(abs(gamestate.vector_entities.at(i)->velY) < STATIC_FRICTION) //if it's going too slow to beat static friction (to avoid oscillation)		
		//if(sqrt(pow(gamestate.vector_entities.at(i)->velX, 2) + pow(gamestate.vector_entities.at(i)->velY, 2)) < STATIC_FRICTION) //vector math instead of component-based
		{
			gamestate.vector_entities.at(i)->velY = 0;
		}
		else
		{
			if(gamestate.vector_entities.at(i)->velY > 0)
			{
				gamestate.vector_entities.at(i)->forceY = gamestate.vector_entities.at(i)->forceY - FRICTION;
			} else
				if(gamestate.vector_entities.at(i)->velY < 0)
				{
					gamestate.vector_entities.at(i)->forceY += FRICTION;
				}
		}
	}
}

void updateHUD(Gamestate& gamestate, HUD& hud)
{
	//update advancedMessages for pos
	if(gamestate.vector_players.at(0).posX >= 0) //only for appearance.. can remove this and the next if
		hud.advancedMessages.at(hud.MESSAGE_POSX).set_message("Pos X:  " + to_string(int(gamestate.vector_players.at(0).posX)));
	else			
		hud.advancedMessages.at(hud.MESSAGE_POSX).set_message("Pos X: " + to_string(int(gamestate.vector_players.at(0).posX)));

	if(gamestate.vector_players.at(0).posY >= 0)
		hud.advancedMessages.at(hud.MESSAGE_POSY).set_message("Pos Y:  " + to_string(int(gamestate.vector_players.at(0).posY)));
	else			
		hud.advancedMessages.at(hud.MESSAGE_POSY).set_message("Pos Y: " + to_string(int(gamestate.vector_players.at(0).posY)));
	
	//update advancedMessages for vel
	if(gamestate.vector_players.at(0).velX >= 0) //the if is just to keep the velocity's from jumping over one when it's negative.. for looks. Unnecessary performance hit here
		hud.advancedMessages.at(hud.MESSAGE_VELX).set_message("Vel X:  " + to_string(int(gamestate.vector_players.at(0).velX)));
	else
		hud.advancedMessages.at(hud.MESSAGE_VELX).set_message("Vel X: " + to_string(int(gamestate.vector_players.at(0).velX)));

	if(gamestate.vector_players.at(0).velY >= 0)
		hud.advancedMessages.at(hud.MESSAGE_VELY).set_message("Vel Y:  " + to_string(int(gamestate.vector_players.at(0).velY)));
	else
		hud.advancedMessages.at(hud.MESSAGE_VELY).set_message("Vel Y: " + to_string(int(gamestate.vector_players.at(0).velY)));
}

void movement(Gamestate& gamestate)
{
	for(int i = 0; i < gamestate.vector_entities.size(); i++)
	{
		//test for boundary collisions; only update position if no boundary collision with said update
		double
			nextX = gamestate.vector_entities.at(i)->posX + gamestate.vector_entities.at(i)->velX,
			nextY = gamestate.vector_entities.at(i)->posY + gamestate.vector_entities.at(i)->velY;

		vector<bool> bad = goodNextPosition(gamestate, nextX, nextY, gamestate.vector_entities.at(i));
		enum { x, y };

		//update positions
		if( !bad.at(x) )
		{
			gamestate.vector_entities.at(i)->posX = nextX;
		}
		if( !bad.at(y) )
		{
			gamestate.vector_entities.at(i)->posY = nextY;
		}
	}
}

void limitVelocity(Gamestate& gamestate)
{
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
}

void forceToVelocity(Gamestate& gamestate)
{	
	for(int i = 0; i < gamestate.vector_players.size(); i++)
	{
		if(gamestate.vector_players.at(i).mass != 0)
		{
			gamestate.vector_players.at(i).velX = gamestate.vector_players.at(i).velX + gamestate.vector_players.at(i).forceX / gamestate.vector_players.at(i).mass; //Vf = Vi + F/m (Vf = Vi + at => Vf = Vi + F/m*1)
			gamestate.vector_players.at(i).velY = gamestate.vector_players.at(i).velY + gamestate.vector_players.at(i).forceY / gamestate.vector_players.at(i).mass;
		}
		gamestate.vector_players.at(i).forceX = 0;
		gamestate.vector_players.at(i).forceY = 0;	
	}
}

void AIVel(Gamestate& gamestate)
{
	double angle, deltaX, deltaY;
	for(int i = 1; i < gamestate.vector_entities.size(); i++)
	{
		//first check if this entity is a monster (if it wants to attack)
		//if it is a monster, check if it's already aggressive. If it is, add velocity in the direction of the player
		//if it isn't, check if the player is in its aggressive radius.
		//if the player is, set it as aggressive and add velocity in the direction of the player
		//if the player isn't, don't do anything

		//add velocity in the direction of the player
		
		//good guy (player) - bad guy entity[i]
		deltaX =  
				( gamestate.vector_entities.at(0)->posX 
				+ gamestate.vector_entities.at(0)->base_posX 
				+ gamestate.vector_entities.at(0)->base_w / 2 ) //the x of the center of the player's bounding box
				- 
				( gamestate.vector_entities.at(i)->posX 
				+ gamestate.vector_entities.at(i)->base_posX
				+ gamestate.vector_entities.at(i)->base_w / 2 );

		//good guy (player) - bad guy entity[i]
		deltaY =  
				( gamestate.vector_entities.at(0)->posY 
				+ gamestate.vector_entities.at(0)->base_posY 
				+ gamestate.vector_entities.at(0)->base_h / 2 ) //the x of the center of the player's bounding box
				- 
				( gamestate.vector_entities.at(i)->posY 
				+ gamestate.vector_entities.at(i)->base_posY
				+ gamestate.vector_entities.at(i)->base_h / 2 );

		angle = atan2(deltaY, deltaX);
		
		gamestate.vector_entities.at(i)->velX = gamestate.vector_entities.at(i)->maxVel * cos(angle);
		gamestate.vector_entities.at(i)->velY = gamestate.vector_entities.at(i)->maxVel * sin(angle);
		OutputDebugString(("cos(angle)[" + to_string(i) + "]: " + to_string(cos(angle)) + "\n").c_str());
		OutputDebugString(("sin(angle)[" + to_string(i) + "]: " + to_string(sin(angle)) + "\n\n").c_str());

	}
}

void Physics::doPhysics(Gamestate& gamestate, HUD& hud, GUI& gui)
{	
	//consider keyboard events for the player
	keyboardInput(gamestate);
	
	//convert force to velocity
	forceToVelocity(gamestate);

	//limit velocity
	limitVelocity(gamestate);

	//give the NPCs velocity with AI
	AIVel(gamestate);

	toggleNPCTextures(gamestate);

	//calculate movement (need to limit position to bounds of window)
	movement(gamestate);

	//update the advanced messages
	updateHUD(gamestate, hud);

	//check if MapDoor_Boundary was entered, if so load a new map
	resolveDoorCollisions(gamestate, gui, hud);

	//add friction as a force opposing the velocity
	friction(gamestate);
}