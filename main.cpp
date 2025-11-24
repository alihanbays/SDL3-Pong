#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <sstream>

class Texture {
    public:
        Texture();
        ~Texture();
        bool loadFromFile(const std::string &path);
        void destroy();
        void render(float x, float y, const SDL_FRect *clip, float newWidth, float newHeight) const;
        bool createTextureFromText(const std::string &text);
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
Texture myTexture;
constexpr SDL_Color bgColor {0, 0, 0, 255};
Uint64 startTime {0};
std::stringstream timeText;
TTF_Font *font {nullptr};


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

void Texture::render(const float x, const float y, const SDL_FRect *clip = nullptr, const float newWidth = 0, const float newHeight = 0) const {

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

    if (sdlTexture == nullptr) {
        SDL_Log("WTF bro");
    }
    SDL_RenderTexture(renderer, sdlTexture, clip, &dRect);
}

bool Texture::createTextureFromText(const std::string &text) {
    destroy();
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), text.size(), white);
    if (textSurface == nullptr) {
        SDL_Log("Text surface can not be initialized! SDL error: %s\n", SDL_GetError());
        return false;
    }

    sdlTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    if (sdlTexture == nullptr) {
        SDL_Log("Texture could not be initialized! SDL error: %s\n", SDL_GetError());
        return false;
    }

    width = textSurface->w;
    height = textSurface->h;

    SDL_DestroySurface(textSurface);
    return true;
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

    if (!TTF_Init()) {
        SDL_Log("TTF could not be initialized! SDL error: %s\n", SDL_GetError());
        success = false;
    }

    return success;
}

bool loadMedia() {
    bool success {true};

    std::string fontPath {"./DirektorCondensed.ttf"};
    font = TTF_OpenFont( fontPath.c_str(), 30 );
    if (font == nullptr){
        SDL_Log( "Could not load %s! SDL_ttf Error: %s\n", fontPath.c_str(), SDL_GetError() );
        success = false;
    }
    return success;
}

void close() {
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
    loadMedia();
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

        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_SPACE) {
                startTime = SDL_GetTicks();
            }
        }
        std::string timePassed  = std::to_string((SDL_GetTicks() - startTime) / 1000);
        myTexture.createTextureFromText(timePassed);
        myTexture.render((ScreenWidth - myTexture.getWidth()) / 2.f, (ScreenHeight - myTexture.getHeight()) / 2.f);
        SDL_RenderPresent(renderer);
    }

    close();
    return exitCode;
}