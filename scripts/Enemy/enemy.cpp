#include "enemy.h"
#include "../game.h"
#include "../constant.h"
#include "../render/resources.h"
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()

Enemy::Enemy(float startX, float startY, SDL_Renderer* renderer) :
    x(startX), y(startY), speed(1), texture(nullptr), timer(60) {
    texture = loadTexture("assets/images/player.png", renderer);
    if (texture == nullptr) {
        SDL_Log("Failed to load enemy texture!");
         //Don't try to render if we don't have a texture.
        return;
    }
    //Seed the random number generator (only once)
    static bool seeded = false;
    if(!seeded)
    {
        srand(time(NULL));
        seeded = true;
    }
}

Enemy::~Enemy() {
    //SDL_DestroyTexture(texture);
}

void Enemy::update() {
    timer--;
    // Simple AI: Move randomly
    if (timer  <= 0) {
        timer = 60;
        int direction = rand() % 4; //0: Up, 1: Down, 2: Left, 3: Right

        switch (direction) {
            case 0:
                y -= speed;
            break;
            case 1:
                y += speed;
            break;
            case 2:
                x -= speed;
            break;
            case 3:
                x += speed;
            break;
        }
    }
}

void Enemy::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_Rect destRect = {static_cast<int>(x) * TILE_SIZE + CENTER_X,static_cast<int>(y) * TILE_SIZE + CENTER_Y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }
    //SDL_Log("enemies %d %d",static_cast<int>(x), static_cast<int>(y));
}

float Enemy::getX() const {
    return x;
}

float Enemy::getY() const {
    return y;
}