#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL3/SDL.h>

class Texture {
    public:
        Texture();
        ~Texture() = default;
        bool generateScoreTexture();
        void render(float x, float y);
        int getHeight();
        int getWidth();
    private:
        SDL_Texture* texture;
        int width;
        int height;
};

#endif