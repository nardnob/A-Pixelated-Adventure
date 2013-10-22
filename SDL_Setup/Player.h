#ifndef PLAYER_H
#define PLAYER_H
#include "Entity.h"

class Player: public Entity
{
private:
	bool
		keyDown[4];
public:
	double 
		velX,
		velY,
		forceX,
		forceY,
		mass;

	void pressKey(int toPress);
	void releaseKey(int toRelease);
	bool keyIsDown(int i);
	void toggleTexture(int toChoose);

	Player();
	Player(int clipX_in, int clipY_in);
};

#endif