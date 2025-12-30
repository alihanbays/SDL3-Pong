#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <SDL3/SDL.h>
#include "../Headers/texture.h"

class GameState {
public:
    virtual bool enter() = 0;
    virtual bool exit() = 0;
    virtual void handleEvent(SDL_Event &e) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual ~GameState() = default;
};

#endif