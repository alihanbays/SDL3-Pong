#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SDL3/SDL.h>


constexpr int ScreenWidth {640};
constexpr int ScreenHeight {480};
constexpr int ScreenFps = 60;
constexpr SDL_Color bgColor {0, 0, 0, 255};
const int targetFps = 60;
const uint64_t targetFrameNs = 1000000000 / targetFps;

#endif