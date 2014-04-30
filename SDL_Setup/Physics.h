#ifndef PHYSICS_H
#define PHYSICS_H
#include "Boundary.h"
#include "Gamestate.h"
#include "GUI.h"
#include "HUD.h"
#include "Player.h"
#include "TerrainMap.h"
#include <vector>
using namespace std;

class Physics 
{
public:
	static void doPhysics(Gamestate& gamestate, HUD& hud, GUI& gui);
	static void updateHealthbarPos(Gamestate* gamestate, int i);
private:
	static bool badPosition(double inX, double inY, vector<Boundary>& boundaries, Entity* entity, vector<Entity*>& entityVector, Gamestate& gamestate);
	static vector<bool> goodNextPosition(Gamestate& gamestate, double nextX, double nextY, Entity* entity);
	static void keyboardInput(Gamestate& gamestate);
	static void toggleNPCTextures(Gamestate& gamestate);
	static void resolveDoorCollisions(Gamestate& gamestate, GUI& gui, HUD& hud);
	static void friction(Gamestate& gamestate);
	static void updateHUD(Gamestate& gamestate, HUD& hud);
	static void movement(Gamestate& gamestate);
	static void limitVelocity(Gamestate& gamestate);
	static void forceToVelocity(Gamestate& gamestate);
	static void AIVel(Gamestate& gamestate);
	static void deathCheck(Gamestate& gamestate/*, HUD& hud, GUI& gui*/);
	static void walkingAnimations(Gamestate& gamestate);
	static void handleAbilities(Gamestate& gamestate);
	static void degradeAnimations(Gamestate& gamestate);
};

#endif