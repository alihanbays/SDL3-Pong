#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <sstream>
#include <optional>

class Box {
    public:
        Box();
        ~Box() = default;
        int boxWidth = 20;
        int boxHeight = 20;
        int maxVelocity {1};
        bool visible {true};
        int checkCollision(SDL_Rect a, SDL_Rect b);
        void controlPlayer(SDL_Event &event);

        void controlPlayer2(SDL_Event &event);

        void movePlayer();
        void move(std::optional<SDL_Rect> collider1, std::optional<SDL_Rect> collider2);
        void render();
        void handleEvent(SDL_Event &event);
        void setSpawnLocation(int x, int y);
        void setSize(int width, int height);
        void serveBall();
        SDL_Rect* getCollisionBox();
        void setMaxVelocity(int maxVelocity);
        void setVelocity(int newX,  int newY);
    private:
        SDL_Rect collisionBox;
        int xVelocity;
        int yVelocity;
};

void Box::setSpawnLocation(int x, int y) {
    collisionBox.x = x;
    collisionBox.y = y;
}

void Box::setSize(int width, int height) {
    collisionBox.w = width;
    collisionBox.h = height;
}

void Box::serveBall() {
    switch (rand() % 4) {
        case 0:
            yVelocity = maxVelocity;
            xVelocity = maxVelocity;
            break;
        case 1:
            yVelocity = maxVelocity * -1;
            xVelocity = maxVelocity * -1;
            break;
        case 2:
            yVelocity = maxVelocity * -1;
            xVelocity = maxVelocity;
            break;
        case 3:
            yVelocity = maxVelocity;
            xVelocity = maxVelocity * -1;
    }
}

SDL_Rect* Box::getCollisionBox() {
    return &collisionBox;
}

void Box::setMaxVelocity(int newVelocity) {
    maxVelocity = newVelocity;
}

void Box::setVelocity(int newX, int newY) {
    xVelocity = newX;
    yVelocity = newY;
}

constexpr int ScreenWidth {640};
constexpr int ScreenHeight {480};
SDL_Window* window {nullptr};
SDL_Renderer* renderer {nullptr};
constexpr SDL_Color bgColor {0, 0, 0, 255};
constexpr int capFps = 60;
int score {0};

void close() {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

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
            case SDLK_SPACE:
                yVelocity = maxVelocity;
                xVelocity = maxVelocity;
                break;
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

int Box::checkCollision(SDL_Rect a, SDL_Rect b) {
    int aXMin { a.x };
    int aXMax { a.x + a.w };
    int aYMin { a.y };
    int aYMax { a.y + a.h };
    int bXMin { b.x };
    int bXMax { b.x + b.w };
    int bYMin { b.y };
    int bYMax { b.y + b.h };

    if (aXMax <= bXMin) {
        return 0;
    }

    if (bXMax <= aXMin) {
        return 0;
    }

    if (aYMax <= bYMin) {
        return 0;
    }

    if (bYMax <= aYMin) {
        return 0;
    }

    //SDL_Log("Collision detected!");
    int yOverlap = std::min(aYMax, bYMax) - std::max(aYMin, bYMin);
    int xOverlap = std::min(aXMax, bXMax) - std::max(aXMin, bXMin);

    if (xOverlap < yOverlap) {
        SDL_Log("Collision detected on X axis");
        return 1;
    }

    if (yOverlap < xOverlap) {
        SDL_Log("Collision detected on Y axis");
        return 2;
    }
    return 3;
}

void Box::controlPlayer(SDL_Event &event) {
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_UP: yVelocity -= maxVelocity; break;
            case SDLK_DOWN: yVelocity += maxVelocity; break;
        }
    }

    if (event.type == SDL_EVENT_KEY_UP && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_UP: yVelocity += maxVelocity; break;
            case SDLK_DOWN: yVelocity -= maxVelocity; break;
        }
    }
}

void Box::controlPlayer2(SDL_Event &event) {
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_W: yVelocity -= maxVelocity; break;
            case SDLK_S: yVelocity += maxVelocity; break;
        }
    }

    if (event.type == SDL_EVENT_KEY_UP && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_W: yVelocity += maxVelocity; break;
            case SDLK_S: yVelocity -= maxVelocity; break;
        }
    }
}

void Box::movePlayer() {
    collisionBox.x += xVelocity;
    collisionBox.y += yVelocity;

    if ((collisionBox.x < 0) || collisionBox.x + collisionBox.w > ScreenWidth) {
        collisionBox.x -= xVelocity;
    }

    if (collisionBox.y < 0 || collisionBox.y + collisionBox.h > ScreenHeight) {
        collisionBox.y -= yVelocity;
    }
}

void Box::move(std::optional<SDL_Rect> collider1 = std::nullopt, std::optional<SDL_Rect> collider2 = std::nullopt) {
    collisionBox.x += xVelocity;
    collisionBox.y += yVelocity;

    if ((collisionBox.x + collisionBox.w < 0) || collisionBox.x > ScreenWidth) {
        visible = false;
    }

    if (collisionBox.y < 0 || collisionBox.y + collisionBox.h > ScreenHeight) {
        yVelocity = yVelocity * -1;
    }

    if (collider1|| collider2) {
        int res = checkCollision(collisionBox, collider1.value());
        int res2 = checkCollision(collisionBox, collider2.value());

        if (res == 1) xVelocity = xVelocity * -1;
        else if (res == 2) yVelocity = yVelocity * -1;

        if (res2 == 1) xVelocity = xVelocity * -1;
        else if (res2 == 2) yVelocity = yVelocity * -1;
    }

}

void Box::render() {
    SDL_FRect drawRect { static_cast<float>(collisionBox.x), static_cast<float>(collisionBox.y), static_cast<float>(collisionBox.w), static_cast<float>(collisionBox.h)};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &drawRect);
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

    int spawnX = (ScreenWidth - myBox.boxWidth) / 2;
    int spawnY = (ScreenHeight - myBox.boxHeight) / 2;
    myBox.setSpawnLocation(spawnX, spawnY);
    // SDL_Rect wall { (ScreenWidth - 40), (ScreenHeight - 150) / 2, 20, 150 };
    // SDL_Rect wall2 { 20, (ScreenHeight - 150) / 2, 20, 150 };

    Box player1;
    player1.setSize(20, 150);
    player1.setSpawnLocation(20, (ScreenHeight - 150) / 2);
    SDL_Rect *playerRect = player1.getCollisionBox();

    Box player2;
    player2.setSize(20, 150);
    player2.setSpawnLocation((ScreenWidth - 40), (ScreenHeight - 150) / 2);
    SDL_Rect *player2Rect = player2.getCollisionBox();

    //myBox.setMaxVelocity(2);
    myBox.serveBall();
    while (quit == false) {
        while (SDL_PollEvent(&event) == true) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            player1.controlPlayer2(event);
            player2.controlPlayer(event);
        }
        if (myBox.visible == false) {
            myBox.visible = true;
            SDL_Delay(1000);
            score += 1;
            myBox.setVelocity(0,0);
            score += 1;
            myBox.setSpawnLocation(spawnX, spawnY);
            myBox.serveBall();
            SDL_Log("Score: %d", score / 2);
        }
        myBox.move(*playerRect, *player2Rect);
        player1.movePlayer();
        player2.movePlayer();
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);
        myBox.render();
        player1.render();
        player2.render();
        SDL_RenderPresent(renderer);
        constexpr Uint64 nsPerFrame = 1000000000 / capFps;
        if (const Uint64 frameNs {SDL_GetTicksNS()}; frameNs < nsPerFrame) {
            SDL_Delay(nsPerFrame - frameNs);
        }
    }

    close();
    return exitCode;
}