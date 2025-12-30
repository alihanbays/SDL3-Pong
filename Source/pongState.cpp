#include "../Headers/pongState.h"
#include "../Headers/constants.h"
#include "../Headers/globals.h"
#include "../Headers/exitState.h"
#include "../Headers/util.h"



#include "../Headers/introState.h"

PongState PongState::pongState;

PongState::PongState() {}

PongState* PongState::get() {
    return &pongState;
}

bool PongState::enter() {
    // Set what will happen when we enter this stage, load texts and pictures
    bool success {true};
    
    int spawnX = (ScreenWidth - ball.boxWidth) / 2;
    int spawnY = (ScreenHeight - ball.boxHeight) / 2;
    ball.setSpawnLocation(spawnX, spawnY);
    player1.setSize(20, 120);
    player1.setSpawnLocation(20, (ScreenHeight - 150) / 2);
    player2.setSize(20, 120);
    player2.setSpawnLocation((ScreenWidth - 40), (ScreenHeight - 150) / 2);
    ball.setMaxVelocity(4);
    ball.serveBall();

    return success;
}

bool PongState::exit() {
    // Determine what happens when we exit this stage, unload textures etc..
    player1.destroy();
    player2.destroy();
    ball.destroy();
    return true;
}

void PongState::handleEvent(SDL_Event &e) {
    // Handle the events that needs to happen. For this I will not have any events or just skip the intro with return key
    player1.controlPlayer2(e);
    player2.controlPlayer(e);
    

    if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) {
        SDL_Log("Escape has been pressed");
        setNextState(ExitState::get());
    }
}

void PongState::render() {
    // Handle the rendering of this state
    // load the score
    if (!scoreTextures[0].loadFromRenderedText(std::to_string(score[0])) || !scoreTextures[1].loadFromRenderedText(std::to_string(score[1]))) {
        SDL_Log("Failed to create texture");
    }

    // Render Logic
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    float middle = (ScreenWidth - scoreTextures[2].getWidth()) / 2.f;
    scoreTextures[0].render(middle - 25.f, scoreTextures[2].getHeight());
    scoreTextures[1].render(middle + 30.f, scoreTextures[2].getHeight());
    scoreTextures[2].render(middle, scoreTextures[2].getHeight());
    ball.render();
    player1.render();
    player2.render();
}

void PongState::update() {
    // handle the update, update the state
    int spawnX = (ScreenWidth - ball.boxWidth) / 2;
    int spawnY = (ScreenHeight - ball.boxHeight) / 2;
    if (ball.visible == false) {
        ball.reset(spawnX, spawnY);
    }

        // Move logic
    ball.move(player1.getCollisionBox(), player2.getCollisionBox());
    player1.movePlayer();
    player2.movePlayer();
}

