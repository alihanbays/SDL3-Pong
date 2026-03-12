#include "../Headers/globals.h"

SDL_Window* window {nullptr};
SDL_Renderer* renderer {nullptr};
int score[2] {0, 0};
// int hitScale {0};
TTF_Font* Font14;
TTF_Font* Font28;
Texture scoreTextures[3] { Texture(), Texture(), Texture()};
GameState *currentState {nullptr};
GameState *nextState {nullptr};
bool running {true};
bool singleplayer {true};
