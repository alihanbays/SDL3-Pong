#include "../Headers/menuState.h"
#include "../Headers/globals.h"
#include "../Headers/pongState.h"
#include "../Headers/constants.h"
#include "../Headers/util.h"

MenuState MenuState::menuState;

MenuState::MenuState() {};

MenuState* MenuState::get() {
    return &menuState;
}

bool MenuState::enter() {
    bool success {true};

    // Title message
    if (messageTexture.loadFromRenderedText("Pong", Font28) == false) {
        SDL_Log("Failed to create title message");
        success = false;
    }

    //  message
    if (messageTexture2.loadFromRenderedText("(1) Singleplayer  (2) Multiplayer", Font14) == false) {
        SDL_Log("Failed to create texture");
        success = false;
    }

    return success;
}

bool MenuState::exit() {
    messageTexture.destroy();
    messageTexture2.destroy();
    return true;
}

void MenuState::render() {
    messageTexture.render((ScreenWidth - messageTexture.getWidth())/ 2.f, (ScreenHeight - messageTexture.getHeight()) / 4.f);
    messageTexture2.render((ScreenWidth - messageTexture2.getWidth())/ 2.f, (ScreenHeight - messageTexture2.getHeight()) / 2.f);
}

void MenuState::handleEvent(SDL_Event &e) {
    // Handle the events that needs to happen. For this I will not have any events or just skip the intro with return key

    // Press 1 for 1 player mode pres 2 for 2 player mode
    // if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_RETURN) {
    //     setNextState(PongState::get());
    // }
    
    if (e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.key == SDLK_1) {
            singleplayer = true;
            setNextState(PongState::get());
        } else if (e.key.key == SDLK_2) {
            singleplayer = false;
            setNextState(PongState::get());
        }
    }
}

void MenuState::update() {
    //TODO
}