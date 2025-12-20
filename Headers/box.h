#ifndef BOX_H
#define BOX_H

#include <SDL3/SDL.h>
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
        void setScore();
    private:
        SDL_Rect collisionBox;
        int xVelocity;
        int yVelocity;
};

#endif