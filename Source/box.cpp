#include "../Headers/box.h"
#include "../Headers/constants.h"
#include "../Headers/globals.h"

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

    collisionBox.x += xVelocity;
    collisionBox.y += yVelocity;
}

void Box::render() {
    SDL_FRect drawRect { static_cast<float>(collisionBox.x), static_cast<float>(collisionBox.y), static_cast<float>(collisionBox.w), static_cast<float>(collisionBox.h)};
    SDL_RenderFillRect(renderer, &drawRect);
}

void Box::reset(int x, int y) {
    visible = true;
    SDL_Delay(1000);
    score += 1;
    setVelocity(0,0);
    setSpawnLocation(x, y);
    serveBall();
}