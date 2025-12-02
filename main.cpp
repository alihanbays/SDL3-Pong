#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <sstream>

class Box {
    public:
        Box();
        ~Box() = default;
        int boxWidth = 20;
        int boxHeight = 20;
        int maxVelocity {4};
        bool visible {true};

        static int checkCollision(SDL_Rect *a, SDL_Rect *b);
        void controlPlayer(SDL_Event &event);
        void controlPlayer2(SDL_Event &event);
        void movePlayer();
        void move(SDL_Rect *collider1, SDL_Rect *collider2);
        void render();

        void reset(int x, int y);

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
int hitScale {0};

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

int Box::checkCollision(SDL_Rect *a, SDL_Rect *b) {
    // a is the ball, b is the paddles
    const int aXMin { a->x };
    const int aXMax { a->x + a->w };
    const int aYMin { a->y };
    const int aYMax { a->y + a->h };
    const int bXMin { b->x };
    const int bXMax { b->x + b->w };
    const int bYMin { b->y };
    const int bYMax { b->y + b->h };

    //SDL_Log("Paddle Y center: %i", bYMax - 76);
    //SDL_Log("Ball Y center: %i", aYMax - 11);



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

    const int yOverlap = std::min(aYMax, bYMax) - std::max(aYMin, bYMin);
    const int xOverlap = std::min(aXMax, bXMax) - std::max(aXMin, bXMin);

    if (xOverlap < yOverlap) {
        if (aXMax > bXMax) {
            // the ball is on the left
            hitScale = (bYMax - 76) - (aYMax - 11);
            a->x += xOverlap;
        } else {
            // the ball is on the right
            hitScale = (bYMax - 76) - (aYMax - 11);
            a->x -= xOverlap;
        }
        return 1;
    }

    if (yOverlap < xOverlap) {
        if (aYMax < bYMax) {
            a->y -= yOverlap;
        } else {
            a->y += yOverlap;
        }
        return 2;
    }
    return 3;
}

void Box::controlPlayer(SDL_Event &event) {
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_UP: yVelocity -= maxVelocity; break;
            case SDLK_DOWN: yVelocity += maxVelocity; break;
            default: break;
        }
    }

    if (event.type == SDL_EVENT_KEY_UP && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_UP: yVelocity += maxVelocity; break;
            case SDLK_DOWN: yVelocity -= maxVelocity; break;
            default: break;
        }
    }
}

void Box::controlPlayer2(SDL_Event &event) {
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_W: yVelocity -= maxVelocity; break;
            case SDLK_S: yVelocity += maxVelocity; break;
            default: break;
        }
    }

    if (event.type == SDL_EVENT_KEY_UP && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_W: yVelocity += maxVelocity; break;
            case SDLK_S: yVelocity -= maxVelocity; break;
            default: break;
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

void Box::move(SDL_Rect *collider1, SDL_Rect *collider2) {
    if ((collisionBox.x + collisionBox.w < 0) || collisionBox.x > ScreenWidth) {
        visible = false;
    }

    if (collisionBox.y < 0 || collisionBox.y + collisionBox.h > ScreenHeight) {
        yVelocity = yVelocity * -1;
    }

    const int res = checkCollision(&collisionBox, collider1);
    const int res2 = checkCollision(&collisionBox, collider2);

    if (res == 1) {
        xVelocity = xVelocity * -1;
        int scaled = abs(hitScale) / 19; // scale from -76..76 to -4..4
        if (yVelocity < 0) {
            scaled *= -1;
        }
        int newX = xVelocity + 1;
        if (newX > 10) newX = 10;
        setVelocity(newX, scaled);
    }
    else if (res == 2) yVelocity = yVelocity * -1;

    if (res2 == 1) {
        xVelocity = xVelocity * -1;
        int scaled = abs(hitScale) / 19; // scale from -76..76 to -4..4
        if (yVelocity < 0) {
            scaled *= -1;
        }
        int newX = xVelocity - 1;
        if (newX < -10) newX = -10;
        setVelocity(newX, scaled);
    }
    else if (res2 == 2) yVelocity = yVelocity * -1;

    SDL_Log("xVelocity: %i", xVelocity);
    collisionBox.x += xVelocity;
    collisionBox.y += yVelocity;
}

void Box::render() {
    SDL_FRect drawRect { static_cast<float>(collisionBox.x), static_cast<float>(collisionBox.y), static_cast<float>(collisionBox.w), static_cast<float>(collisionBox.h)};
    SDL_RenderFillRect(renderer, &drawRect);
}

void Box::reset(int x, int y) {
    visible = true;
    //SDL_Delay(1000);
    score += 1;
    setVelocity(0,0);
    setSpawnLocation(x, y);
    serveBall();
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

int main() {
    int exitCode {0};
    if (init() == false) {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
        close();
        return exitCode;
    }
    constexpr Uint64 nsPerFrame = 1000000000 / capFps;
    Uint64 lastTicks = SDL_GetTicksNS();
    bool quit {false};
    SDL_Event event;
    SDL_zero(event);
    Box ball;

    int spawnX = (ScreenWidth - ball.boxWidth) / 2;
    int spawnY = (ScreenHeight - ball.boxHeight) / 2;
    ball.setSpawnLocation(spawnX, spawnY);

    Box player1;
    player1.setSize(20, 150);
    player1.setSpawnLocation(20, (ScreenHeight - 150) / 2);
    SDL_Rect *playerRect = player1.getCollisionBox();

    Box player2;
    player2.setSize(20, 150);
    player2.setSpawnLocation((ScreenWidth - 40), (ScreenHeight - 150) / 2);
    SDL_Rect *player2Rect = player2.getCollisionBox();

    ball.setMaxVelocity(4);
    ball.serveBall();
    while (quit == false) {
        const Uint64 frameStart = SDL_GetTicksNS();
        Uint64 deltaNs    = frameStart - lastTicks;
        lastTicks         = frameStart;
        while (SDL_PollEvent(&event) == true) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            player1.controlPlayer2(event);
            player2.controlPlayer(event);
        }
        if (ball.visible == false) {
            ball.reset(spawnX, spawnY);
        }
        ball.move(playerRect, player2Rect);
        player1.movePlayer();
        player2.movePlayer();
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        ball.render();
        player1.render();
        player2.render();
        SDL_RenderPresent(renderer);

        if (const Uint64 frameTime = SDL_GetTicksNS() - frameStart; frameTime < nsPerFrame) {
            SDL_Delay((nsPerFrame - frameTime) / 1000000);
        }
    }

    close();
    return exitCode;
}