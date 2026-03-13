#define SDL_MAIN_USE_CALLBACKS 1
#include "Headers/box.h"
#include "Headers/constants.h"
#include "Headers/exitState.h"
#include "Headers/globals.h"
#include "Headers/introState.h"
#include "Headers/pongState.h"
#include "Headers/texture.h"
#include "Headers/util.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <sstream>
#include <string>

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (init() == false)
    {
        SDL_Log("Unable to initialize program!\n");
        return SDL_APP_FAILURE;
    }

    currentState = IntroState::get();
    if (currentState->enter() == false)
    {
        currentState->exit();
        currentState = ExitState::get();
    }

    frameStart = SDL_GetTicksNS();
    return SDL_APP_CONTINUE;
}

// Input
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    currentState->handleEvent(*event);

    if (event->type == SDL_EVENT_QUIT)
    {
        setNextState(ExitState::get());
    }

    return SDL_APP_CONTINUE;
}

// Update
SDL_AppResult SDL_AppIterate(void *appstate)
{
    frameStart = SDL_GetTicksNS();

    currentState->update();

    if (changeState() == false)
    {
        currentState->exit();
        currentState = ExitState::get();
    }

    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(renderer);

    currentState->render();

    SDL_RenderPresent(renderer);

    frameEnd = SDL_GetTicksNS();
    frameTime = frameEnd - frameStart;

    if (frameTime < targetFrameNs)
    {
        uint64_t delayNs = targetFrameNs - frameTime;
        SDL_DelayNS(delayNs);
    }

    if (currentState == ExitState::get())
    {
        SDL_Log("Stopping game");
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    close();
}
