#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>

class Texture {
    public:
        Texture();
        ~Texture();
        bool loadFromFile(const std::string &path);
        void destroy();
        bool isLoaded() const;
        void render(float x, float y) const;
        int getHeight() const;
        int getWidth() const;
        int setHeight(int newHeight);
        int setWidth(int newWidth);

    private:
        SDL_Texture* sdlTexture;
        int height;
        int width;
};

constexpr int ScreenWidth {640};
constexpr int ScreenHeight {480};
SDL_Window* window {nullptr}; 
SDL_Renderer* renderer {nullptr};
Texture pngTextures[4];
constexpr SDL_Color bgColor {0xFF, 0xFF, 0xFF, 0xFF};


Texture::Texture():
    sdlTexture{nullptr},
    height{0},
    width{0}
{
}

Texture::~Texture() {
    destroy();
}

bool Texture::loadFromFile(const std::string &path) {
    destroy();

    SDL_Surface* surface = IMG_Load(path.c_str());
    
    if (surface == nullptr) {
        SDL_Log("Surface could not be initialized! SDL error: %s\n", SDL_GetError());
        return false;
    }

    const bool trans = SDL_SetSurfaceColorKey(surface, true, SDL_MapSurfaceRGB(surface, 0x00, 0xFF, 0xFF));
    if (!trans) {
        SDL_Log("Color key could not be initialized! SDL error: %s\n", SDL_GetError());
        return false;
    }
    sdlTexture = SDL_CreateTextureFromSurface(renderer, surface);

    if (sdlTexture == nullptr) {
        SDL_Log("Texture could not be initialized! SDL error: %s\n", SDL_GetError());
        return false;
    }

    height = surface->h;
    width = surface->w;

    SDL_DestroySurface(surface);

    return true;
} 

void Texture::destroy() {
    SDL_DestroyTexture(sdlTexture);
    sdlTexture = nullptr;
    height = 0;
    width = 0;
}

void Texture::render(const float x, const float y) const {
    const SDL_FRect rect{x, y, static_cast<float>(width), static_cast<float>(height)};
    SDL_RenderTexture(renderer, sdlTexture, nullptr, &rect);
}

int Texture::getHeight() const {
    return height;
}

int Texture::getWidth() const {
    return width;
}

int Texture::setHeight(const int newHeight) {
    return height = newHeight;
}

int Texture::setWidth(const int newWidth) {
    return width = newWidth;
}

bool Texture::isLoaded() const {
    return sdlTexture != nullptr;
}

bool init() {
    bool success {true};

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not be initialized! SDL error: %s\n", SDL_GetError());
        success = false;
    } 

    if (!SDL_CreateWindowAndRenderer("Pong", ScreenWidth, ScreenHeight, 0, &window, &renderer)) {
        SDL_Log("SDL could not create window and renderer! SDL error: %s\n", SDL_GetError());
        success = false;
    }

    return success;
}

bool loadMedia() {
    bool success {true};

    if (pngTextures[0].loadFromFile("masterpiece.png") == false) {
        SDL_Log("Unable to load image! SDL error: %s\n", SDL_GetError());
        success = false;
    }

    if (pngTextures[1].loadFromFile("man.png") == false) {
        SDL_Log("Unable to load image! SDL error: %s\n", SDL_GetError());
        success = false;
    }

    return success;
}

void close() {
    for (auto & pngTexture : pngTextures) {
        pngTexture.destroy();
    }

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

int moveDirection() {
    const bool *keyStates = SDL_GetKeyboardState(nullptr);

    if (keyStates[SDL_SCANCODE_UP]) return 0;
    if (keyStates[SDL_SCANCODE_DOWN]) return 1;
    if (keyStates[SDL_SCANCODE_LEFT]) return 2;
    if (keyStates[SDL_SCANCODE_RIGHT]) return 3;

    return 5;
}

int main(int argc, char* args[]) {
    int exitCode {0};

    if (init() == false) {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
        close();
        return exitCode;
    }

    if (loadMedia() == false) {
        SDL_Log("Unable to load media!\n");
        exitCode = 2;
        close();
        return exitCode;
    }
    
    bool quit {false};
    SDL_Event event;
    SDL_zero(event);
    pngTextures[1].setWidth(pngTextures[1].getWidth() / 2);
    pngTextures[1].setHeight(pngTextures[1].getHeight() / 2);

    while (quit == false) {
        while (SDL_PollEvent(&event) == true) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);
        pngTextures[0].render(0.f, 0.f);
        pngTextures[1].render(static_cast<float>(ScreenWidth - pngTextures[1].getWidth()) / 2.f,
                              (static_cast<float>(ScreenHeight - pngTextures[1].getHeight())) / 2.f);
        SDL_RenderPresent(renderer);
    }

    close();
    return exitCode;
}