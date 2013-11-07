#ifndef NPC_H
#define NPC_H

#include "Entity.h"

class NPC: public Entity
{
private:
	
public:

	void toggleTexture(int toChoose);

	NPC();
	NPC(int in_clipX, int in_clipY, double in_posX, double in_posY, double in_base_posX, double in_base_posY, double in_base_w, double in_base_h);
};

#endif