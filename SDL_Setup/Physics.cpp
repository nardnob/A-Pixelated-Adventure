#include "AnimationEntity.h"
#include "AnimationStatic.h"
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

//#define PI 3.14159265

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

bool Physics::badPosition(double inX, double inY, vector<Boundary>& boundaries, Entity* entity, vector<Entity*>& entityVector, Gamestate& gamestate)
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
				if(entityVector.at(i) == entityVector.at(0)) //if the entity (the one checking for collisions) collides with the player
				{
					gamestate.vector_players.at(0).currentStatus.takeLife(
						entity->playerCollisionDamage()
						);
				}
				return true;
			}
		}
	}

	return false;
}

vector<bool> Physics::goodNextPosition(Gamestate& gamestate, double nextX, double nextY, Entity* entity)
{
	vector<bool> bad;
	bad.push_back(false);
	bad.push_back(false);
	enum { x, y };

	if( badPosition(nextX, entity->posY, gamestate.currentMap.boundaries, entity, gamestate.vector_entities, gamestate) )
		bad.at(x) = true;

	if( badPosition(entity->posX, nextY, gamestate.currentMap.boundaries, entity, gamestate.vector_entities, gamestate) )
		bad.at(y) = true;

	return bad;
}

void Physics::toggleNPCTextures(Gamestate& gamestate)
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

void Physics::resolveDoorCollisions(Gamestate& gamestate, GUI& gui, HUD& hud)
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
				gui.switchMap(gamestate.currentMap.mapDoor_boundaries.at(i).toMap, gamestate.currentMap.mapDoor_boundaries.at(i).toX, gamestate.currentMap.mapDoor_boundaries.at(i).toY);
			}

			done = true;
		}
		else
		{
			gamestate.currentMap.mapDoor_boundaries.at(i).inTheDoorway = false;
		}
	}
	
}

void Physics::friction(Gamestate& gamestate)
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

void Physics::updateHUD(Gamestate& gamestate, HUD& hud)
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

	hud.advancedMessages.at(hud.MESSAGE_LIFE).set_message("Life:  " + to_string(int(gamestate.vector_players.at(0).currentStatus.lifeAmount())));

	hud.hudMessages.at(0).set_message(to_string(int(ceil(gamestate.vector_players.at(0).currentStatus.lifeAmount()))) + " / " + 
									  to_string(int(gamestate.vector_players.at(0).currentStatus.get_maxLife())));
	hud.hudMessages.at(1).set_message(to_string(int(ceil(gamestate.vector_players.at(0).currentStatus.manaAmount()))) + " / " + 
									  to_string(int(gamestate.vector_players.at(0).currentStatus.get_maxMana())));
}

void Physics::updateHealthbarPos(Gamestate* gamestate, int i)
{
	//update the entitie's healthbar positions
	gamestate->vector_entities.at(i)->healthBar.x = gamestate->vector_entities.at(i)->posX + HEALTHBAR_ENTITY_OFFSET_X;
	gamestate->vector_entities.at(i)->healthBar.y = gamestate->vector_entities.at(i)->posY + HEALTHBAR_ENTITY_OFFSET_Y;
	gamestate->vector_entities.at(i)->healthBar_BG.x = gamestate->vector_entities.at(i)->posX + HEALTHBAR_ENTITY_OFFSET_X;
	gamestate->vector_entities.at(i)->healthBar_BG.y = gamestate->vector_entities.at(i)->posY + HEALTHBAR_ENTITY_OFFSET_Y;
	gamestate->vector_entities.at(i)->healthBar_border.x = gamestate->vector_entities.at(i)->posX + HEALTHBAR_BORDER_ENTITY_OFFSET_X;
	gamestate->vector_entities.at(i)->healthBar_border.y = gamestate->vector_entities.at(i)->posY + HEALTHBAR_BORDER_ENTITY_OFFSET_Y;
}

void Physics::movement(Gamestate& gamestate)
{
	for(int i = 0; i < gamestate.vector_entities.size(); i++)
	{
		//test for boundary collisions; only update position if no boundary collision with said update
		double
			nextX = gamestate.vector_entities.at(i)->posX + gamestate.vector_entities.at(i)->velX,
			nextY = gamestate.vector_entities.at(i)->posY + gamestate.vector_entities.at(i)->velY,
			movedX = 0,
			movedY = 0;

		vector<bool> bad = goodNextPosition(gamestate, nextX, nextY, gamestate.vector_entities.at(i));
		enum { x, y };

		//update positions
		if( !bad.at(x) )
		{
			movedX = nextX - gamestate.vector_entities.at(i)->posX;
			gamestate.vector_entities.at(i)->posX = nextX;
		}
		if( !bad.at(y) )
		{
			movedY = nextY - gamestate.vector_entities.at(i)->posY;
			gamestate.vector_entities.at(i)->posY = nextY;
		}

		updateHealthbarPos(&gamestate, i);

		if(i != 0)
		{
			//update walking variables for non-player entities based on change in position
			if(sqrt(pow(movedX, 2) + pow(movedY, 2)) > .1) //if a non player entity has moved
			{
				//start walking if it's the beginning of walking, not a continuation
				if(!gamestate.vector_entities.at(i)->walking)
				{
					gamestate.vector_entities.at(i)->walking = true;
					gamestate.vector_entities.at(i)->toWalk = gamestate.vector_entities.at(i)->walkSpeed;
					gamestate.vector_entities.at(i)->walkUpNext = true;
					gamestate.vector_entities.at(i)->spriteOffsetY = ENTITY_CLIP_H;
				}
			}
			else //else stop walking if not moving
			{
				gamestate.vector_entities.at(i)->walking = false;
				gamestate.vector_entities.at(i)->spriteOffsetY = 0;
			}
		}
	}
}

void Physics::limitVelocity(Gamestate& gamestate)
{
	for(int i = 0; i < gamestate.vector_players.size(); i++)
	{
		double currentVel = sqrt(pow(gamestate.vector_players.at(i).velX, 2) + pow(gamestate.vector_players.at(i).velY, 2));

		if(currentVel > gamestate.vector_players.at(i).maxVel)
		{
			//divide each component by magnitude to create a unit vector
			//multiply each component by desired magnitude (max vel in this case)
			gamestate.vector_players.at(i).velX /= (currentVel * 1 / gamestate.vector_players.at(i).maxVel);
			gamestate.vector_players.at(i).velY /= (currentVel * 1 / gamestate.vector_players.at(i).maxVel);
		}

		/*
		//This limits per vector component (x and y), meaning diagonal is faster than horizontal or vertical movement
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
		*/
	}
}

void Physics::forceToVelocity(Gamestate& gamestate)
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

void Physics::AIVel(Gamestate& gamestate)
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
		//OutputDebugString(("cos(angle)[" + to_string(i) + "]: " + to_string(cos(angle)) + "\n").c_str());
		//OutputDebugString(("sin(angle)[" + to_string(i) + "]: " + to_string(sin(angle)) + "\n\n").c_str());

	}
}

//check for player death and act on it
void Physics::deathCheck(Gamestate& gamestate/*, HUD& hud, GUI& gui*/)
{
	if(!gamestate.vector_players.at(0).currentStatus.hasLife())
	{
		gamestate.switchState(STATES_DEATH_MENU);
	}
	
	for(int i = gamestate.vector_entities.size() - 1; i >= 1; i--)
	{
		if(!gamestate.vector_entities.at(i)->currentStatus.hasLife())
		{
			gamestate.vector_animations.push_back(new AnimationStatic(
				gamestate.vector_entities.at(i)->posX + ENTITY_CLIP_W / 2 - ANIMATION_CLIP_W / 2,
				gamestate.vector_entities.at(i)->posY + ENTITY_CLIP_H / 2 - ANIMATION_CLIP_H / 2,
				ANIMATION_NUM_GRAVE,
				ANIMATION_DEGRADATIONRATE_GRAVE
				));

			//delete any relevant animations for this entitie's death
			for(int j = gamestate.vector_animations.size() - 1; j >= 1; j--)
			{
				if(gamestate.vector_animations.at(j)->get_ownerEntity() == gamestate.vector_entities.at(i))
				{
					delete gamestate.vector_animations.at(j);
					gamestate.vector_animations.erase(gamestate.vector_animations.begin() + j);
				}
			}

			gamestate.vector_entities.erase(gamestate.vector_entities.begin() + i);
		}
	}
}

void Physics::walkingAnimations(Gamestate& gamestate)
{
	for(int i = 0; i < gamestate.vector_entities.size(); i++)
	{
		if(gamestate.vector_entities.at(i)->walking)
		{
			if(gamestate.vector_entities.at(i)->toWalk == 0)
			{
				if(gamestate.vector_entities.at(i)->spriteOffsetY != 0)
				{
					gamestate.vector_entities.at(i)->spriteOffsetY = 0;
				}
				else
				{
					if(gamestate.vector_entities.at(i)->walkUpNext)
					{
						gamestate.vector_entities.at(i)->walkUpNext = false;
						gamestate.vector_entities.at(i)->spriteOffsetY = ENTITY_CLIP_H;
					}
					else
					{
						gamestate.vector_entities.at(i)->walkUpNext = true;
						gamestate.vector_entities.at(i)->spriteOffsetY = -ENTITY_CLIP_H;
					}
				}

				gamestate.vector_entities.at(i)->toWalk = gamestate.vector_entities.at(i)->walkSpeed;
			}

			gamestate.vector_entities.at(i)->toWalk--;
		}
	}
}

void Physics::regenStats(Gamestate& gamestate)
{
	gamestate.vector_players.at(0).currentStatus.giveLife(REGEN_LIFE_PERFRAME);
	gamestate.vector_players.at(0).currentStatus.giveMana(REGEN_MANA_PERFRAME);
}

void Physics::degradeAnimations(Gamestate& gamestate)
{
	for(int i = gamestate.vector_animations.size() - 1; i >= 0; i--)
	{
		gamestate.vector_animations.at(i)->degrade();
		if(gamestate.vector_animations.at(i)->anticipatesDeath())
		{
			delete gamestate.vector_animations.at(i);
			gamestate.vector_animations.erase(gamestate.vector_animations.begin() + i);			
		}
	}
}

void Physics::handleAbilities(Gamestate& gamestate)
{
	for(int i = 0; i < gamestate.vector_abilities_player.size(); i++)
	{ 
		//use the ability from the abilities vector
		if(gamestate.vector_abilities_player.at(i)->useAbility())
		{
			//add any animations associated with the ability to the animation vector
			switch(gamestate.vector_abilities_player.at(i)->get_animationType())
			{
			case ANIMATION_TYPE_NONE: //do not add an animation
				break;
			case ANIMATION_TYPE_STATIC:
				gamestate.vector_animations.push_back(new AnimationStatic(
					gamestate.vector_abilities_player.at(i)->ownerEntity->posX + ENTITY_CLIP_W / 2 - ANIMATION_CLIP_W / 2,
					gamestate.vector_abilities_player.at(i)->ownerEntity->posY + ENTITY_CLIP_H / 2 - ANIMATION_CLIP_H / 2,
					gamestate.vector_abilities_player.at(i)->get_animationNum(),
					gamestate.vector_abilities_player.at(i)->get_animationDegradationRate()
					));
				break;
			case ANIMATION_TYPE_ENTITY:
				gamestate.vector_animations.push_back(new AnimationEntity(
					gamestate.vector_abilities_player.at(i)->ownerEntity->posX + ENTITY_CLIP_W / 2 - ANIMATION_CLIP_W / 2,
					gamestate.vector_abilities_player.at(i)->ownerEntity->posY + ENTITY_CLIP_H / 2 - ANIMATION_CLIP_H / 2,
					gamestate.vector_abilities_player.at(i)->get_animationNum(),
					gamestate.vector_abilities_player.at(i)->get_animationDegradationRate(),
					gamestate.vector_abilities_player.at(i)->ownerEntity
					));
				break;
			}
		}

		//delete the ability and remove it from the ability vector
		delete gamestate.vector_abilities_player.at(i);
		gamestate.vector_abilities_player.pop_back();
	}
}

void Physics::keyboardInput(Gamestate& gamestate)
{
	for(int i = 0; i < KEY_BUFFER_SIZE; i++)
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
			case KEY_SPACE:
				//gamestate.vector_players.at(0).currentStatus.takeLife(1);
				break;
			}
		}
	}
}

void Physics::doPhysics(Gamestate& gamestate, HUD& hud, GUI& gui)
{
	//degrade animations (before possibily adding new animations)
	degradeAnimations(gamestate);

	//consider keyboard events for the player
	keyboardInput(gamestate);

	//handle attack vectors (mob and player)
	handleAbilities(gamestate);
	
	//convert force to velocity
	forceToVelocity(gamestate);

	//limit velocity
	limitVelocity(gamestate);

	//give the NPCs velocity with AI
	AIVel(gamestate);

	toggleNPCTextures(gamestate);

	//calculate movement (need to limit position to bounds of window)
	movement(gamestate);

	walkingAnimations(gamestate);

	//update the advanced messages
	updateHUD(gamestate, hud);

	//check if MapDoor_Boundary was entered, if so load a new map
	resolveDoorCollisions(gamestate, gui, hud);

	//add friction as a force opposing the velocity
	friction(gamestate);

	//check for player death
	deathCheck(gamestate);

	//regenerate player stats (hp / mana / etc)
	regenStats(gamestate);
}