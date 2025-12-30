#include "../Headers/box.h"
#include "../Headers/constants.h"
#include "../Headers/globals.h"
#include "../Headers/util.h"

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

Box::Box():
    collisionBox { 0, 0, boxWidth, boxHeight},
    xVelocity {0},
    yVelocity {0}
{
}

void Box::controlPlayer(SDL_Event &event) {
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_UP: yVelocity -= playerSpeed; break;
            case SDLK_DOWN: yVelocity += playerSpeed; break;
            default: break;
        }
    }

    if (event.type == SDL_EVENT_KEY_UP && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_UP: yVelocity += playerSpeed; break;
            case SDLK_DOWN: yVelocity -= playerSpeed; break;
            default: break;
        }
    }
}

void Box::controlPlayer2(SDL_Event &event) {
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_W: yVelocity -= playerSpeed; break;
            case SDLK_S: yVelocity += playerSpeed; break;
            default: break;
        }
    }

    if (event.type == SDL_EVENT_KEY_UP && event.key.repeat == 0) {
        switch (event.key.key) {
            case SDLK_W: yVelocity += playerSpeed; break;
            case SDLK_S: yVelocity -= playerSpeed; break;
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
        // int scaled = abs(hitScale) / 19; // scale from -76..76 to -4..4
        // if (yVelocity < 0) {
        //     scaled *= -1;
        // }
        int newX = xVelocity + 1;
        if (newX > 10) newX = 10;
        setVelocity(newX, yVelocity);
    }
    else if (res == 2) yVelocity = yVelocity * -1;

    if (res2 == 1) {
        xVelocity = xVelocity * -1;
        // int scaled = abs(hitScale) / 19; // scale from -76..76 to -4..4
        // if (yVelocity < 0) {
        //     scaled *= -1;
        // }
        int newX = xVelocity - 1;
        if (newX < -10) newX = -10;
        setVelocity(newX, yVelocity);
    }
    else if (res2 == 2) yVelocity = yVelocity * -1;

    collisionBox.x += xVelocity;
    collisionBox.y += yVelocity;
}

void Box::render() {
    SDL_FRect drawRect { static_cast<float>(collisionBox.x), static_cast<float>(collisionBox.y), static_cast<float>(collisionBox.w), static_cast<float>(collisionBox.h)};
    SDL_RenderFillRect(renderer, &drawRect);
}

void Box::setScore() {
    if (collisionBox.x  < 0) {
        score[1] += 1;
    } else if (collisionBox.x + collisionBox.w > ScreenWidth) {
        score[0] += 1;
    }
}

void Box::reset(int x, int y) {
    visible = true;
    SDL_Delay(1000);
    setScore();
    setVelocity(0,0);
    setSpawnLocation(x, y);
    serveBall();
}

void Box::destroy() {
    boxWidth = 0;
    boxHeight = 0;
}