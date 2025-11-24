#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <sstream>

class Box {
    public:
        Box();
        int boxHeight {20};
        int boxWidth {20};
        int maxVelocity {5};
        void move();
        void render();
        void handleEvent(SDL_Event &event);
    private:
        SDL_Rect rect;
        int xVelocity;
        int yVelocity;
};

constexpr int ScreenWidth {640};
constexpr int ScreenHeight {480};
SDL_Window* window {nullptr};
SDL_Renderer* renderer {nullptr};
Box myBox;
constexpr SDL_Color bgColor {0, 0, 0, 255};
Uint64 startTime {0};
std::stringstream timeText;
TTF_Font *font {nullptr};
constexpr int capFps = 60;

Box::Box() {
    SDL_Rect rect { 0, 0, boxWidth, boxHeight};
    xVelocity = 0;
    yVelocity = 0;
}

void Box::handleEvent(SDL_Event &event) {
    // -Y is up, -X is left
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_UP: yVelocity -= maxVelocity; break;
            case SDLK_DOWN: yVelocity += maxVelocity; break;
            case SDLK_LEFT: xVelocity -= maxVelocity; break;
            case SDLK_RIGHT: xVelocity += maxVelocity; break;
        }
    }

    if (event.type == SDL_EVENT_KEY_UP && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_UP: yVelocity += maxVelocity; break;
            case SDLK_DOWN: yVelocity -= maxVelocity; break;
            case SDLK_LEFT: xVelocity += maxVelocity; break;
            case SDLK_RIGHT: xVelocity -= maxVelocity; break;
        }
    }
}

void Box::move() {
    rect.x += xVelocity;
    rect.y += yVelocity;

    if (rect.x < 0) rect.x = 0;
    if (rect.y < 0) rect.y = 0;

    // 620 + 20 | 460 + 20 = 640, 480
    if (rect.x + boxHeight > ScreenWidth) rect.x -= maxVelocity;
    if (rect.y + boxHeight > ScreenHeight) rect.y -= maxVelocity;

    SDL_Log("X: %d, Y: %d", rect.x, rect.y);
}

void Box::render() {
    SDL_FRect drawRect { static_cast<float>(rect.x), static_cast<float>(rect.y), static_cast<float>(boxWidth), static_cast<float>(boxHeight)};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &drawRect);
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

    // std::string fontPath {"./DirektorCondensed.ttf"};
    // font = TTF_OpenFont( fontPath.c_str(), 30 );
    // if (font == nullptr){
    //     SDL_Log( "Could not load %s! SDL_ttf Error: %s\n", fontPath.c_str(), SDL_GetError() );
    //     success = false;
    // }

    // if (myTexture.loadFromFile("dots.png") == false) {
    //     SDL_Log("Unable to load image! SDL error: %s\n", SDL_GetError());
    //     success = false;
    // }
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
            myBox.handleEvent(event);
        }
        myBox.move();
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);
        myBox.render();
        SDL_RenderPresent(renderer);

        constexpr Uint64 nsPerFrame = 1000000000 / capFps;
        if (const Uint64 frameNs {SDL_GetTicksNS()}; frameNs < nsPerFrame) {
            SDL_Delay(nsPerFrame - frameNs);
        }
    }

    close();
    return exitCode;
}