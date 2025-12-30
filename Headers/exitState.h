#ifndef EXITSTATE_H
#define EXITSTATE_H
#include <SDL3/SDL.h>
#include "../Headers/gameState.h"
#include "../Headers/texture.h"

class ExitState : public GameState {
    public:
        static ExitState* get();
        bool enter() override;
        bool exit() override;
        void handleEvent(SDL_Event &e) override;
        void update() override;
        void render() override;
    private:
        static ExitState exitState;
        ExitState();
        Texture messageTexture;
        Uint64 startingFrame;
        Uint64 delay;
};

#endif