#ifndef GLOBALS_H
#define GLOBALS_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../Headers/texture.h"
#include "../Headers/gameState.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern int score[];
extern TTF_Font* Font;
extern Texture scoreTextures[];
extern GameState *currentState;
extern GameState *nextState;
extern bool running;

#endif