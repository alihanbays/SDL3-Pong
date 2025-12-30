#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Headers/box.h"
#include "Headers/texture.h"
#include "Headers/globals.h"
#include "Headers/constants.h"
#include "Headers/util.h"
#include "Headers/introState.h"
#include "Headers/pongState.h"
#include "Headers/exitState.h"
#include <string>
#include <sstream>

int main() {
    int exitCode {0};
    if (init() == false) {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
        close();
        return exitCode;
    }

    uint64_t frameStart;
    uint64_t frameEnd;
    uint64_t frameTime;
    SDL_Event event;
    SDL_zero(event);

    currentState = IntroState::get();
    if (currentState->enter() == false) {
        currentState->exit();
        currentState = ExitState::get();
    }

    while (running) {
        frameStart = SDL_GetTicksNS();

        while (SDL_PollEvent(&event) == true) {

            currentState->handleEvent(event);

            if (event.type == SDL_EVENT_QUIT) {
                setNextState(ExitState::get());
            }
        }

        currentState->update();

        if (changeState() == false) {
            currentState->exit();
            currentState = ExitState::get();
        }

        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);
        
        currentState->render();

        SDL_RenderPresent(renderer);

        frameEnd = SDL_GetTicksNS();
        frameTime = frameEnd - frameStart;

        if (frameTime < targetFrameNs) {
            uint64_t delayNs = targetFrameNs - frameTime;
            SDL_DelayNS(delayNs);
        }
    }

    close();
    return exitCode;
}