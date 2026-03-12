#ifndef GLOBALS_H
#define GLOBALS_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../Headers/texture.h"
#include "../Headers/gameState.h"



extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* Font28;
extern TTF_Font* Font14;
extern int score[];
extern Texture scoreTextures[];
extern GameState *currentState;
extern GameState *nextState;
extern bool running;
extern bool singleplayer;


#endif