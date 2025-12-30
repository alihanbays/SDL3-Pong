#include "../Headers/introState.h"
#include "../Headers/util.h"
#include "../Headers/globals.h"
#include "../Headers/constants.h"
#include "../Headers/pongState.h"

IntroState IntroState::introState;

IntroState::IntroState() {}

IntroState* IntroState::get() {
    return &introState;
}

bool IntroState::enter() {
    // Set what will happen when we enter this stage, load texts and pictures

    bool success {true};
    
    if (messageTexture.loadFromRenderedText("Alihan Baysal presents...") == false) {
        SDL_Log("Failed to create intro message");
        success = false;
    }

    startingFrame = SDL_GetTicks();
    delay = 2000;

    return success;
}

bool IntroState::exit() {
    // Determine what happens when we exit this stage, unload textures etc..
    messageTexture.destroy();
    return true;
}

void IntroState::handleEvent(SDL_Event &e) {
    // Handle the events that needs to happen. For this I will not have any events or just skip the intro with return key

    if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_RETURN) {
        setNextState(PongState::get());
    }
    
}

void IntroState::render() {
    messageTexture.render((ScreenWidth - messageTexture.getWidth())/ 2.f, (ScreenHeight - messageTexture.getHeight()) / 2.f);
}

void IntroState::update() {
    if (SDL_GetTicks() - startingFrame >= delay) {
        setNextState(PongState::get());
    }
}