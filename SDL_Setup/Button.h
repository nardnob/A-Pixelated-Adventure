#ifndef BUTTON_H
#define BUTTON_H
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "MenuObject.h"

#include <string>
#include <vector>
using namespace std;

class Button : public MenuObject
{
public:
	SDL_Rect buttonRect;

	int
		rectOffset_y = 0,
		clickId;

	string message;

	vector<int>* clickEvents;

	void defineWidthHeight();
	int get_width();
	int get_height();
	void handleMouseOver(int x, int y);
	void handleMouseDown(int x, int y);
	void handleMouseUp(int x, int y);

	enum { BUTTON_START, BUTTON_OPTIONS, BUTTON_CREDITS, BUTTON_EXIT };

	Button(vector<int>* in_clickEvents, int in_clickId, int in_rectW, int in_rectH, int in_rectX, int in_rectY, 
		   int in_posX, int in_posY, string in_message, TTF_Font* in_font);

private:
	bool
		mouseOver = false,
		mouseDown = false,
		clicked = true;

	TTF_Font* font;

	void updateOffset();
	void onClick(int clickId);
	void display(SDL_Surface* surface_messager, SDL_Surface* surface_buttons, SDL_Surface* surface_screen);

};

#endif