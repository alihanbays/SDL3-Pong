#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>

class Texture {
    public:
        Texture();
        ~Texture();
        static constexpr int originalSize {-1};
        bool loadFromFile(const std::string &path);
        void destroy();
        void render(float x, float y, const SDL_FRect *clip, float width = originalSize, float height = originalSize) const;
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

void Texture::render(const float x, const float y, const SDL_FRect *clip, float newWidth, float newHeight) const {

    // Construct the initial rect
    SDL_FRect dRect{x, y, static_cast<float>(width),  static_cast<float>(height)};

    if (clip != nullptr) {
        dRect.w = clip->w;
        dRect.h = clip->h;
    }

    if (newWidth > 0) {
        dRect.w = newWidth;
    }
    if (newHeight > 0) {
        dRect.h = newHeight;
    }
    SDL_RenderTexture(renderer, sdlTexture, clip, &dRect);
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

    while (quit == false) {
        while (SDL_PollEvent(&event) == true) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);

        constexpr SDL_FRect redCircle{0.f, 0.f, 100, 100};
        constexpr SDL_FRect greenCircle{0.f, 100.f, 100, 100};
        constexpr SDL_FRect yellowCircle{100.f, 0.f, 100, 100};
        constexpr SDL_FRect blueCircle{100.f, 100.f, 100, 100};

        pngTextures[0].render(redCircle.x, redCircle.y, &redCircle);
        pngTextures[0].render(ScreenWidth - greenCircle.w, 0.f, &greenCircle); // Is there a way to not bound this to greenCircle width value and not use hard coded value as well
        pngTextures[0].render(0, ScreenHeight - yellowCircle.h, &yellowCircle);
        pngTextures[0].render(ScreenWidth - blueCircle.w, ScreenHeight - blueCircle.h, &blueCircle);

        SDL_RenderPresent(renderer);
    }

    close();
    return exitCode;
}