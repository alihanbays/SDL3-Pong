#include "../Headers/util.h"
#include "../Headers/globals.h"
#include "../Headers/constants.h"
#include <SDL3/SDL.h>
#include <string>


void close() {
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
    scoreTextures[0].generateScoreTexture(std::to_string(score[0]));
    scoreTextures[1].generateScoreTexture(std::to_string(score[1]));
    scoreTextures[2].generateScoreTexture(":");


    return success;
}
