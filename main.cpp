#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <sstream>

class Box {
    public:
        Box();
        static constexpr int boxWidth = 20;
        static constexpr int boxHeight = 20;
        int maxVelocity {5};

        bool checkCollision(SDL_Rect a, SDL_Rect b);

        void move(SDL_Rect collider);
        void render();
        void handleEvent(SDL_Event &event);
    private:
        SDL_Rect collisionBox;
        int xVelocity;
        int yVelocity;
};

constexpr int ScreenWidth {640};
constexpr int ScreenHeight {480};
SDL_Window* window {nullptr};
SDL_Renderer* renderer {nullptr};
constexpr SDL_Color bgColor {0, 0, 0, 255};
constexpr int capFps = 60;

Box::Box():
    collisionBox { 0, 0, boxWidth, boxHeight},
    xVelocity {0},
    yVelocity {0}
{
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

bool Box::checkCollision(SDL_Rect a, SDL_Rect b) {
    int aXMin { a.x };
    int aXMax { a.x + a.w };
    int aYMin { a.y };
    int aYMax { a.y + a.h };
    int bXMin { b.x };
    int bXMax { b.x + b.w };
    int bYMin { b.y };
    int bYMax { b.y + b.h };


    SDL_Log("Xmin: %d, Xmax: %d, Ymin: %d, Ymax: %d", bXMin, bXMax, bYMin, bYMax);

    if (aXMax <= bXMin) {
        return false;
    }

    if (bXMax <= aXMin) {
        return false;
    }

    if (aYMax <= bYMin) {
        return false;
    }

    if (bYMax <= aYMin) {
        return false;
    }

    return true;
}

void Box::move(SDL_Rect collider) {
    collisionBox.x += xVelocity;
    collisionBox.y += yVelocity;

    if ((collisionBox.x < 0) || collisionBox.x + collisionBox.w > ScreenWidth || checkCollision(collider, collisionBox)) {
        collisionBox.x -= xVelocity;
    }

    if (collisionBox.y < 0 || collisionBox.y + collisionBox.h > ScreenHeight || checkCollision(collider, collisionBox)) {
        collisionBox.y -= yVelocity;
    }
}

void Box::render() {
    SDL_FRect drawRect { static_cast<float>(collisionBox.x), static_cast<float>(collisionBox.y), static_cast<float>(collisionBox.w), static_cast<float>(collisionBox.h)};
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
    bool quit {false};
    SDL_Event event;
    SDL_zero(event);
    Box myBox;
    SDL_Rect wall { (ScreenWidth - 20) / 2, (ScreenHeight - 20) / 2, 20, 200 };
    while (quit == false) {
        while (SDL_PollEvent(&event) == true) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            myBox.handleEvent(event);
        }
        myBox.move(wall);
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);
        myBox.render();

        // Render wall
        SDL_FRect drawWall { static_cast<float>(wall.x), static_cast<float>(wall.y), static_cast<float>(wall.w), static_cast<float>(wall.h)};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &drawWall);

        SDL_RenderPresent(renderer);
        constexpr Uint64 nsPerFrame = 1000000000 / capFps;
        if (const Uint64 frameNs {SDL_GetTicksNS()}; frameNs < nsPerFrame) {
            SDL_Delay(nsPerFrame - frameNs);
        }
    }

    close();
    return exitCode;
}