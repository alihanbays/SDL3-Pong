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
        int setHeight(int height);
        int setWidth(int width);

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
    destroy();

    SDL_Surface* surface = IMG_Load(path.c_str());
    
    if (surface == nullptr) {
        SDL_Log("Surface could not be initilized! SDL error: %s\n", SDL_GetError());
        return false;
    }
    //#00FDFF
    bool trans = SDL_SetSurfaceColorKey(surface, true, SDL_MapSurfaceRGB(surface, 0x00, 0xFF, 0xFF));
    if (!trans) {
        SDL_Log("Color key could not be initilized! SDL error: %s\n", SDL_GetError());
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
    SDL_FRect rect{x, y, static_cast<float>(width), static_cast<float>(height)};
    SDL_RenderTexture(renderer, sdlTexture, nullptr, &rect);
}

int Texture::getHeight() {
    return height;
}

int Texture::getWidth() {
    return width;
}

int Texture::setHeight(int newHeight) {
    return height = newHeight;
}

int Texture::setWidth(int newWidth) {
    return width = newWidth;
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
    
    // if (pngTextures[0].loadFromFile("pngs/up.png") == false) {
    //     SDL_Log("Unable to load image! SDL error: %s\n", SDL_GetError());
    //     success = false;
    // }

    // if (pngTextures[1].loadFromFile("pngs/down.png") == false) {
    //     SDL_Log("Unable to load image! SDL error: %s\n", SDL_GetError());
    //     success = false;
    // }

    // if (pngTextures[2].loadFromFile("pngs/left.png") == false) {
    //     SDL_Log("Unable to load image! SDL error: %s\n", SDL_GetError());
    //     success = false;
    // }

    // if (pngTextures[3].loadFromFile("pngs/right.png") == false) {
    //     SDL_Log("Unable to load image! SDL error: %s\n", SDL_GetError());
    //     success = false;
    // }

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
    for (int i = 0; i < sizeof(pngTextures) / sizeof(pngTextures[0]); i++) {
        pngTextures[i].destroy();
    }

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

int moveDirection() {
    const bool *keystates = SDL_GetKeyboardState(nullptr);

    if (keystates[SDL_SCANCODE_UP]) return 0;
    if (keystates[SDL_SCANCODE_DOWN]) return 1;
    if (keystates[SDL_SCANCODE_LEFT]) return 2;
    if (keystates[SDL_SCANCODE_RIGHT]) return 3;

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
    int direction{0};
    pngTextures[1].setWidth(pngTextures[1].getWidth() / 2.f);
    pngTextures[1].setHeight(pngTextures[1].getHeight() / 2.f);
    // Texture *toRender{nullptr};
    while (quit == false) {
        while (SDL_PollEvent(&event) == true) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }
        // direction = moveDirection();
        SDL_Color bgColor {0xFF, 0xFF, 0xFF, 0xFF};
        // switch (direction)
        // {
        // case 0:
        //     toRender = &pngTextures[0];
        //     bgColor.r = 0xFF;
        //     bgColor.g = 0x00;
        //     bgColor.b = 0x00;
        //     break;
        // case 1:
        //     toRender = &pngTextures[1];
        //     bgColor.r = 0x00;
        //     bgColor.g = 0xFF;
        //     bgColor.b = 0x00;
        //     break;
        // case 2:
        //     toRender = &pngTextures[2];
        //     bgColor.r = 0x00;
        //     bgColor.g = 0x00;
        //     bgColor.b = 0xFF;
        //     break;
        // case 3:
        //     toRender = &pngTextures[3];
        //     bgColor.r = 0x00;
        //     bgColor.g = 0xFF;
        //     bgColor.b = 0xFF;
        //     break;
        // default:
        //     toRender = nullptr;
        // }

        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);
        // if (toRender != nullptr && toRender->isLoaded()) {
        //     toRender->render((ScreenWidth - toRender->getWidth()) / 2.f, (ScreenHeight - toRender->getHeight()) / 2.f);
        // }
        pngTextures[0].render(0.f, 0.f);
        pngTextures[1].render((ScreenWidth - pngTextures[1].getWidth()) / 2.f, (ScreenHeight - pngTextures[1].getHeight()) / 2.f);
        SDL_RenderPresent(renderer);
    }

    close();
    return exitCode;
}