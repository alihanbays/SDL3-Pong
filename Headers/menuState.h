#ifndef MENUSTATE_H
#define MENUSTATE_H
#include "../Headers/gameState.h"
#include "../Headers/texture.h"
#include <vector>

class MenuState : public GameState
{
  public:
    static MenuState *get();
    bool enter() override;
    bool exit() override;
    void handleEvent(SDL_Event &e) override;
    void update() override;
    void render() override;

  private:
    static MenuState menuState;
    MenuState();
    Texture messageTexture;
    Texture messageTexture2;
};

#endif
