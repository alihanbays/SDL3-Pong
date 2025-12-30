#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL3/SDL.h>
#include <string>

class Texture {
    public:
        Texture();
        ~Texture();
        bool loadFromRenderedText(std::string textureText);
        void render(float x, float y);
        void destroy();
        int getHeight();
        int getWidth();
    private:
        SDL_Texture* texture;
        int width;
        int height;
};

#endif