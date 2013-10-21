//Screen attributes
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int SCREEN_BPP = 32;

//terrain file attributes
const int TERRAIN_FILE_W = 512;
const int TERRAIN_FILE_H = 512;
const int TERRAIN_CLIP_W = 32;
const int TERRAIN_CLIP_H = 32;
const int TERRAIN_CLIP_COUNT = 7;

//entity file attributes
const int ENTITY_FILE_W = 512;
const int ENTITY_FILE_H = 512;
const int ENTITY_CLIP_W = 128;
const int ENTITY_CLIP_H = 128;

// constants for function parameters
enum { CODE_TERRAIN, CODE_ENTITY };

//constants for physics
const double FRICTION = 1;
const double STATIC_FRICTION = .1;
const double WALKING_FORCE = 4;

//constants for entities
const double MASS_PLAYER = 20;

//constants for keyboard shortcuts
enum { KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN };