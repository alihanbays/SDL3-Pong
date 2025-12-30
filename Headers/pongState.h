#ifndef PONGSTATE_H
#define PONGSTATE_H
#include <SDL3/SDL.h>
#include "../Headers/gameState.h"
#include "../Headers/box.h"

class PongState : public GameState {
    public:
        static PongState* get();
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
};

#endif 