#ifndef PONGSTATE_H
#define PONGSTATE_H
#include "../Headers/box.h"
#include "../Headers/gameState.h"
#include <SDL3/SDL.h>

class PongState : public GameState
{
  public:
    static PongState *get();
    bool enter() override;
    bool exit() override;
    void handleEvent(SDL_Event &e) override;
    void update() override;
    void render() override;

  private:
    static PongState pongState;
    Box player1;
    Box player2;
    Box ball;
    PongState();
    Uint64 startingFrame;
    Uint64 delay;
};

#endif
