#ifndef UTIL_H
#define UTIL_H
#include "../Headers/gameState.h"
#include <SDL3/SDL.h>

void close();
bool init();
int checkCollision(SDL_Rect *a, SDL_Rect *b);
void setNextState(GameState* state);
bool changeState();

#endif