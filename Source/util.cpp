#include "../Headers/util.h"
#include "../Headers/globals.h"
#include "../Headers/constants.h"
#include "../Headers/gameState.h"
#include "../Headers/exitState.h"
#include <SDL3/SDL.h>
#include <string>


void close() {
    SDL_Log("Trying to Quit...");
    for (int i = 0; i < 3; i ++) {
        scoreTextures[i].destroy();
    }
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

bool init() {
    bool success {true};

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL could not be initialized! SDL error: %s\n", SDL_GetError());
        success = false;
    }

    if (!SDL_CreateWindowAndRenderer("Pong", ScreenWidth, ScreenHeight, 0, &window, &renderer)) {
        SDL_Log("SDL could not create window and renderer! SDL error: %s\n", SDL_GetError());
        success = false;
    }

    if(!TTF_Init()) {
        SDL_Log( "SDL_ttf could not initialize! SDL_ttf error: %s\n", SDL_GetError() );
        success = false;
    }
    
    // Initilize Font
    std::string fontPath{ "../Assets/font.ttf" };
    Font = TTF_OpenFont( fontPath.c_str(), 28 );

    if (Font == nullptr) {
        SDL_Log("generateScoreTexture: Font is null");
        return false;
    }

    // Initilize Scoreboard
    scoreTextures[0].loadFromRenderedText(std::to_string(score[0]));
    scoreTextures[1].loadFromRenderedText(std::to_string(score[1]));
    scoreTextures[2].loadFromRenderedText(":");


    return success;
}

int checkCollision(SDL_Rect *a, SDL_Rect *b) {
    // a is the ball, b is the paddles
    const int aXMin { a->x };
    const int aXMax { a->x + a->w };
    const int aYMin { a->y };
    const int aYMax { a->y + a->h };
    const int bXMin { b->x };
    const int bXMax { b->x + b->w };
    const int bYMin { b->y };
    const int bYMax { b->y + b->h };

    if (aXMax <= bXMin) {
        return 0;
    }

    if (bXMax <= aXMin) {
        return 0;
    }

    if (aYMax <= bYMin) {
        return 0;
    }

    if (bYMax <= aYMin) {
        return 0;
    }

    const int yOverlap = std::min(aYMax, bYMax) - std::max(aYMin, bYMin);
    const int xOverlap = std::min(aXMax, bXMax) - std::max(aXMin, bXMin);

    if (xOverlap < yOverlap) {
        if (aXMax > bXMax) {
            // the ball is on the left
            a->x += xOverlap;
        } else {
            // the ball is on the right
            a->x -= xOverlap;
        }
        return 1;
    }

    if (yOverlap < xOverlap) {
        if (aYMax < bYMax) {
            a->y -= yOverlap;
        } else {
            a->y += yOverlap;
        }
        return 2;
    }
    
    return 3;
}

void setNextState(GameState* newState) {
    if (nextState != ExitState::get()) {
        nextState = newState;
    }
}

bool changeState() {
    bool success {true};

    if (nextState != nullptr) {
        SDL_Log("Changing state to: %p", &nextState);
        success = currentState->exit() && success;
        success = nextState->enter() && success;

        currentState = nextState;
        nextState = nullptr;
    }

    return success;
}
