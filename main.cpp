#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>

class Texture {
    public:
        Texture();
        ~Texture();
        bool loadFromFile(std::string path);
        void destroy();
        bool isLoaded();
        void render(float x, float y);
        int getHeight();
        int getWidth();

    private:
        SDL_Texture* sdlTexture;
        int height;
        int width;
};

constexpr int ScreenWidth {640};
constexpr int ScreenHeight {480};
SDL_Window* window {nullptr}; 
SDL_Surface* screenSurface {nullptr};
SDL_Renderer* renderer {nullptr};
SDL_Surface* helloWorld {nullptr};
Texture pngTexture;

Texture::Texture():
    sdlTexture{nullptr},
    height{0},
    width{0}
{
}

Texture::~Texture() {
    destroy();
}

bool Texture::loadFromFile(std::string path) {
      // If already exists
    destroy();

    SDL_Surface* surface = IMG_Load(path.c_str());
    
    if (surface == nullptr) {
        SDL_Log("Surface could not be initilized! SDL error: %s\n", SDL_GetError());
        return false;
    }

    sdlTexture = SDL_CreateTextureFromSurface(renderer, surface);

    if (sdlTexture == nullptr) {
        SDL_Log("Texture could not be initilized! SDL error: %s\n", SDL_GetError());
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

void Texture::render(float x, float y) {
    SDL_FRect rect{x, y, static_cast<float>(height), static_cast<float>(width)};
    SDL_RenderTexture(renderer, sdlTexture, nullptr, &rect);
}

int Texture::getHeight() {
    return height;
}

int Texture::getWidth() {
    return width;
}

bool Texture::isLoaded() {
    return sdlTexture != nullptr;
}

bool init() {
    bool success {true};

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not be initilized! SDL error: %s\n", SDL_GetError());
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

    if (pngTexture.loadFromFile("hello-sdl3.bmp") == false) {
        SDL_Log("Unable to load image! SDL error: %s\n", SDL_GetError());
        success = false;
    }
    
    // std::string imagePath {"./hello-sdl3.bmp"};

    // helloWorld = SDL_LoadBMP(imagePath.c_str()) ;

    // if (helloWorld == nullptr) {
    //     SDL_Log("Unable to load image! SDL error: %s\n", SDL_GetError());
    // }

    return success;
}

void close() {
    pngTexture.destroy();
    SDL_DestroySurface(helloWorld);
    helloWorld = nullptr;
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    screenSurface = nullptr;

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
        while (SDL_PollEvent(&event) == true) { // If there are events to process
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        pngTexture.render(0.f, 0.f);
        SDL_RenderPresent(renderer);
    }

    close();
    return exitCode;
}