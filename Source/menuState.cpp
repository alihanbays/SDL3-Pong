#include "../Headers/menuState.h"

MenuState MenuState::menuState;

MenuState::MenuState() {};

MenuState* MenuState::get() {
    return &menuState;
}

bool MenuState::enter() {
    bool success {true};

    // Title message
    if (messageTexture.loadFromRenderedText("Pong") == false) {
        SDL_Log("Failed to create score message");
        success = false;
    }

    //  message
    if (messageTexture.loadFromRenderedText("(1) 1 Player      (2) 2 Players") == false) {
        SDL_Log("Failed to create score message");
        success = false;
    }

    return success;
}ß

bool S