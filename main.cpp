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
        void render(const SDL_FRect *srcRect, const SDL_FRect *dstRect) const;
        bool isLoaded() const;
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

void Texture::render(const SDL_FRect *srcRect, const SDL_FRect *dstRect) const {
    SDL_RenderTexture(renderer, sdlTexture, srcRect, dstRect);
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

    // if (pngTextures[0].loadFromFile("masterpiece.png") == false) {
    //     SDL_Log("Unable to load image! SDL error: %s\n", SDL_GetError());
    //     success = false;
    // }
    //
    // if (pngTextures[1].loadFromFile("man.png") == false) {
    //     SDL_Log("Unable to load image! SDL error: %s\n", SDL_GetError());
    //     success = false;
    // }
    if (pngTextures[0].loadFromFile("dots.png") == false) {
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
    SDL_Log( std::to_string(pngTextures[0].getWidth()).c_str());
    SDL_Log(std::to_string(pngTextures[0].getHeight()).c_str());

    while (quit == false) {
        while (SDL_PollEvent(&event) == true) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);
        // Lets try to grab the first red circle
        const SDL_FRect redCircle{0.f, 0.f, 100, 100};
        const SDL_FRect greenCircle{0.f, 100.f, 100, 100};
        const SDL_FRect yellowCircle{100.f, 0.f, 100, 100};
        const SDL_FRect blueCircle{100.f, 100.f, 100, 100};

        const SDL_FRect dstRect{
            0.f,
            0.f,
            static_cast<float>(pngTextures[0].getWidth()),
            static_cast<float>(pngTextures[0].getHeight())
        };

        const SDL_FRect leftTop{
            0,
            0,
            100,
            100
        };
        const SDL_FRect rightTop{
            ScreenWidth - 50,
            0,
            50,
            100
        };
        const SDL_FRect leftBottom{
            0,
            ScreenHeight - 100,
            100,
            100
        };
        const SDL_FRect rightBottom{
            ScreenWidth - 100,
            ScreenHeight - 50,
            100,
            50
        };

        pngTextures[0].render(&redCircle, &leftTop);
        pngTextures[0].render(&greenCircle, &rightTop);
        pngTextures[0].render(&yellowCircle, &leftBottom);
        pngTextures[0].render(&blueCircle, &rightBottom);

        SDL_RenderPresent(renderer);
    }

    close();
    return exitCode;
}