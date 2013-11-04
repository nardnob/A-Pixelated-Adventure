#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
using namespace std;

//Screen attributes
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND = 100;
const string ICON_FILE = "icon2.bmp";

//HUD attributes
const int HUD_HEIGHT = 160;
const SDL_Color FONT_COLOR_WHITE = { 255,255,255 };
const int FONT_HUD_1_SIZE = 24;
const char FONT_HUD_1_FILENAME[] = "font_HUD_3.ttf";
//const int NUM_ADVANCED_SETTINGS = 1;//no longer needed. Was using to loop through advanced settings, which are now stored in a vector (can do .size())

//terrain file attributes
const int TERRAIN_FILE_W = 512;
const int TERRAIN_FILE_H = 512;
const int TERRAIN_CLIP_W = 32;
const int TERRAIN_CLIP_H = 32;
const int TERRAIN_CLIP_COUNT = 8;

//entity file attributes
const int ENTITY_FILE_W = 512;
const int ENTITY_FILE_H = 512;
const int ENTITY_CLIP_W = 64;
const int ENTITY_CLIP_H = 64;

// constants for function parameters
enum { CODE_TERRAIN, CODE_ENTITY, CODE_HUD };

enum { TEXTURE_FRONT, TEXTURE_BACK, TEXTURE_LEFT, TEXTURE_RIGHT };

//constants for physics
const double FRICTION = 3;
const double STATIC_FRICTION = 2;
const double WALKING_FORCE = 4;

//constants for entities
const double MASS_PLAYER = 1;
const double MASS_NPC = 1;
const double BASE_POSX = 17; //for the player
const double BASE_POSY = 41; //for the player
const double BASE_W = 30; //for the player
const double BASE_H = 23; //For the player


//constants for keyboard shortcuts
enum { KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN };