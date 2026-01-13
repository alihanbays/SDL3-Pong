#include "../Headers/scoreState.h"
#include "../Headers/texture.h"
#include "../Headers/exitState.h"
#include "../Headers/util.h"
#include "../Headers/constants.h"
#include "../Headers/globals.h"

ScoreState ScoreState::scoreState;

ScoreState::ScoreState() {};

ScoreState* ScoreState::get() {
    return &scoreState;
}

bool ScoreState::enter() {
    bool success {true};

    if (score[1] == 10) {
        if (messageTexture.loadFromRenderedText("Player 2 Win!") == false) {
            SDL_Log("Failed to create score message");
            success = false;
        }
    } else {
        if (messageTexture.loadFromRenderedText("Player 1 Win!") == false) {
            SDL_Log("Failed to create score message");
            success = false;
        }
    }

    startingFrame = SDL_GetTicks();
    delay = 2000;

    return success;
}

bool ScoreState::exit() {
    messageTexture.destroy();
    return true;
}

void ScoreState::handleEvent(SDL_Event &e) {
    if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_RETURN) {
        setNextState(ExitState::get());
    }
}

void ScoreState::render() {
    messageTexture.render((ScreenWidth - messageTexture.getWidth())/ 2.f, (ScreenHeight - messageTexture.getHeight()) / 2.f);
}

void ScoreState::update() {
    if (SDL_GetTicks() - startingFrame >= delay) {
        delay = 0;
        setNextState(ExitState::get());
    }
}

