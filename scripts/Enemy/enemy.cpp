#include "enemy.h"
#include "../map.h"
#include "../game.h"
#include "../constant.h"
#include "../logic.h"
#include "../render/resources.h"
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <cmath>
#include <queue>

Enemy::Enemy(float X, float Y, SDL_Renderer* renderer) :
    x0(X), y0(Y), speed(SPEED_PLAYER), texture(nullptr), timer(60), find(1), step(0), save(0) {
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

bool Enemy::is_valid(const Map& map,const int& x,const int& y) const {
    return (x >= 0) && (x < map.getWidth()) && (y >= 0) && (y < map.getHeight()) && (!(map.limit(x, y) > '0'));
}


void Enemy::update(int target_x, int target_y, const Map& map) {
    next_dx = 0;
    next_dy = 0;
    timer--;
    if (timer <= 0) {
        timer = 15;
        int dx[] = {-1, 0, 1, 0};
        int dy[] = {0, -1, 0, 1};
        double vx = target_x - x0;
        double vy = target_y - y0;
        double distance = hypot(vx, vy);
        SDL_Log("%f %f %d" ,vx, vy, save);
        if (vx <= 0 && vy < 0 && save != 0) {
            find = 0;
            save = 0;
        } else if (vx > 0 && vy <= 0 && save != 1) {
            find = 1;
            save = 1;
        } else if (vx >= 0 && vy > 0 && save != 2) {
            find = 2;
            save = 2;
        } else if (vx < 0 && vy >= 0 && save != 3) {
            find = 3;
            save = 3;
        }

        while (!is_valid(map,static_cast<int>(x0 + dx[find]),static_cast<int>(y0 + dy[find]))) {
            find = (find + 1)%4;
        }
        if (next_dx < 1 && next_dy < 1) {
            next_dx = dx[find];
            next_dy = dy[find];
        }
        if (find != save) find = (find - 1)%4;
        if (find < 0) find = 4 + find;
    }
}


void Enemy::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_Rect destRect = {static_cast<int>(x0 * TILE_SIZE) + CENTER_X,static_cast<int>(y0 * TILE_SIZE) + CENTER_Y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }
}

double Enemy::getX() const {
    return x0;
}

double Enemy::getY() const {
    return y0;
}

double Enemy::get_dx() const {
    return next_dx;
}

double Enemy::get_dy() const {
    return next_dy;
}


