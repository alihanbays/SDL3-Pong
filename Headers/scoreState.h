#ifndef SCORESTATE_H
#define SCORESTATE_H
#include "../Headers/gameState.h"
#include "../Headers/texture.h"

class ScoreState : public GameState
{
  public:
    static ScoreState *get();
    bool enter() override;
    bool exit() override;
    void handleEvent(SDL_Event &e) override;
    void update() override;
    void render() override;

  private:
    static ScoreState scoreState;
    ScoreState();
    Texture messageTexture;
    Uint64 startingFrame;
    Uint64 delay;
};

#endif
