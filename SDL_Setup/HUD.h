#ifndef HUD_H
#define HUD_H
#include "SDL.h"
#include "SDL_ttf.h"

#include "Message.h"
#include <vector>
using namespace std;

class HUD {
private:	
	bool advanced; //showing advanced system info in the HUD (vel, pos, etc)
public:
	HUD();

	SDL_Rect HUD_rect; //the brown box that is the HUD area
	SDL_Rect HUD_rect_overlay; //the inside brown box of the HUD area (effectively creates a border)
	SDL_Rect healthBar; //the green foreground of the health bar; should decrease with health
	SDL_Rect healthBar_BG; //the red background of the health bar
	SDL_Rect manaBar; //the blue foreground of the mana bar; should decrease with mana
	SDL_Rect manaBar_BG; //the background of the mana bar

	TTF_Font* font_HUD_1;
	vector<Message> advancedMessages; //all of the advanced messages of the HUD. only displayed if advanced (if f3)
	vector<Message> hudMessages; //Messages for the hud, such as the health on the health bar	

	enum { MESSAGE_VELX, MESSAGE_VELY, MESSAGE_POSX, MESSAGE_POSY, MESSAGE_FPS, MESSAGE_CURRENTMAP, MESSAGE_LIFE };
	
	void toggleAdvanced();
	bool get_advanced() { return this->advanced; }
};

#endif