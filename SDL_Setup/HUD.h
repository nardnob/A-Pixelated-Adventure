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

	SDL_Rect HUD_rect; //the gray box that is the HUD area
	TTF_Font* font_HUD_1;
	vector<Message> advancedMessages; //all of the advanced messages of the HUD. only displayed if advanced (if f3)
	
	enum { MESSAGE_VELX, MESSAGE_VELY, MESSAGE_POSX, MESSAGE_POSY, MESSAGE_FPS };
	
	void toggleAdvanced();
	bool get_advanced() { return this->advanced; }
};

#endif