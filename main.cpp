#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Headers/box.h"
#include "Headers/texture.h"
#include "Headers/globals.h"
#include "Headers/constants.h"
#include "Headers/util.h"
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
    constexpr Uint64 nsPerFrame = 1000000000 / ScreenFps;
    Uint64 lastTicks = SDL_GetTicksNS();
    bool quit {false};
    SDL_Event event;
    SDL_zero(event);
    Box ball;

    int spawnX = (ScreenWidth - ball.boxWidth) / 2;
    int spawnY = (ScreenHeight - ball.boxHeight) / 2;
    ball.setSpawnLocation(spawnX, spawnY);

    Box player1;
    player1.setSize(20, 150);
    player1.setSpawnLocation(20, (ScreenHeight - 150) / 2);
    SDL_Rect *playerRect = player1.getCollisionBox();

    Box player2;
    player2.setSize(20, 150);
    player2.setSpawnLocation((ScreenWidth - 40), (ScreenHeight - 150) / 2);
    SDL_Rect *player2Rect = player2.getCollisionBox();

    ball.setMaxVelocity(4);
    ball.serveBall();
    while (quit == false) {
        const Uint64 frameStart = SDL_GetTicksNS();
        Uint64 deltaNs    = frameStart - lastTicks;
        lastTicks         = frameStart;
        while (SDL_PollEvent(&event) == true) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            player1.controlPlayer2(event);
            player2.controlPlayer(event);
        }
        if (ball.visible == false) {
            ball.reset(spawnX, spawnY);
        }
        // load the score
        if (!scoreTexture.generateScoreTexture()) {
            SDL_Log("Failed to create texture");
        }

        ball.move(playerRect, player2Rect);
        player1.movePlayer();
        player2.movePlayer();
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        scoreTexture.render((ScreenWidth - scoreTexture.getWidth()) / 2.f, scoreTexture.getHeight());
        ball.render();
        player1.render();
        player2.render();
        SDL_RenderPresent(renderer);

        if (const Uint64 frameTime = SDL_GetTicksNS() - frameStart; frameTime < nsPerFrame) {
            SDL_Delay((nsPerFrame - frameTime) / 1000000);
        }
    }

    close();
    return exitCode;
}