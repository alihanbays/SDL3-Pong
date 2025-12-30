#ifndef INTROSTATE_H
#define INTROSTATE_H
#include <SDL3/SDL.h>
#include "../Headers/gameState.h"
#include "../Headers/texture.h"

class IntroState : public GameState {
    public:
        static IntroState* get();
        bool enter() override;
        bool exit() override;
        void handleEvent(SDL_Event &e) override;
        void update() override;
        void render() override;
    private:
        static IntroState introState;
        IntroState();
        Texture messageTexture;
        Uint64 startingFrame;
        Uint64 delay;
};

#endif