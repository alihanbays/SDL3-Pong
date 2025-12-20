#include "../Headers/texture.h"
#include "../Headers/globals.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

Texture::Texture():
    texture {nullptr},
    width {0},
    height {0}
{
}

bool Texture::generateScoreTexture(std::string printScore) {
    SDL_Color textColor = { 255, 255, 255, 255 };
    std::string scoreText = printScore;
    SDL_Surface *surface = TTF_RenderText_Blended(Font, scoreText.c_str(), scoreText.size(), textColor);

    if (surface == nullptr) {
        SDL_Log("generateScoreTexture: surface is null");
        return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == nullptr) {
        SDL_Log("generateScoreTexture: texture null");
        return false;
    }

    width = surface->w;
    height = surface->h;

    SDL_DestroySurface(surface);
    return texture != nullptr;
}

void Texture::render(float x, float y) {
    //Set texture position
    SDL_FRect dstRect{ x, y, static_cast<float>( width ), static_cast<float>( height ) };

    //Render texture
    SDL_RenderTexture(renderer, this->texture, nullptr, &dstRect );
}

int Texture::getHeight() {
    return height;
}

int Texture::getWidth() {
    return width;
}