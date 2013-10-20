#include "Physics.h"
#include "Player.h"
#include "constants.cpp"
#include <vector>
#include <cmath>
using namespace std;

void Physics::doPhysics(vector<Player>& inVector)
{	
	//consider keyboard events for the player
	for(int i = 0; i < 4; i++)
	{
		if(inVector.at(0).keyIsDown(i))
		{
			switch(i)
			{
			case KEY_LEFT:
				inVector.at(0).forceX -= WALKING_FORCE;
				break;
			case KEY_RIGHT:
				inVector.at(0).forceX += WALKING_FORCE;
				break;
			case KEY_UP:
				inVector.at(0).forceY -= WALKING_FORCE;
				break;
			case KEY_DOWN:
				inVector.at(0).forceY += WALKING_FORCE;
				break;
			}
		}
	}

	//calculate collisions


	//resolve collisions

	
	//convert force to velocity
	for(int i = 0; i < inVector.size(); i++)
	{
		if(inVector.at(i).mass != 0)
		{
			inVector.at(i).velX = inVector.at(i).velX + inVector.at(i).forceX / inVector.at(i).mass; //Vf = Vi + F/m (Vf = Vi + at => Vf = Vi + F/m*1)
			inVector.at(i).velY = inVector.at(i).velY + inVector.at(i).forceY / inVector.at(i).mass;
		}
		inVector.at(i).forceX = 0;
		inVector.at(i).forceY = 0;
	}

	//calculate movement (need to limit position to bounds of window)
	for(int i = 0; i < inVector.size(); i++)
	{
		inVector.at(i).posX += inVector.at(i).velX;
		inVector.at(i).posY += inVector.at(i).velY;
	}

	////calculate friction
	//for(int i = 0; i < inVector.size(); i++)
	//{
	//	//calculating friction for the x axis
	//	if((inVector.at(i).velX < 1 && inVector.at(i).velX > 0) || (inVector.at(i).velX > -1 && inVector.at(i).velX < 0)) //if it's going too slow to beat static friction (to avoid oscillation)
	//	{
	//		inVector.at(i).velX = 0;
	//	}
	//	else
	//	{
	//		if(inVector.at(i).velX > 0)
	//		{
	//			inVector.at(i).forceX -= FRICTION;
	//		} else
	//			if(inVector.at(i).velX < 0)
	//			{
	//				inVector.at(i).forceX += FRICTION;
	//			}
	//	}

	//	//calculating friction for the y axis
	//	if(abs(inVector.at(i).velY) < STATIC_FRICTION) //if it's going too slow to beat static friction (to avoid oscillation)
	//	{
	//		inVector.at(i).velY = 0;
	//	}
	//	else
	//	{
	//		if(inVector.at(i).velY > 0)
	//		{
	//			inVector.at(i).forceY -= FRICTION;
	//		} else
	//			if(inVector.at(i).velY < 0)
	//			{
	//				inVector.at(i).forceY += FRICTION;
	//			}
	//	}
	//}
}