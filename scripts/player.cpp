
#include "player.h"
#include <SDL.h>  // Required for SDL_Log

Player::Player(int startX, int startY) : x(startX), y(startY), speed(1), bombLimit(1) {}

void Player::move(int dx, int dy) {
    x += dx;
    y += dy;
    SDL_Log("Player moved to (%d, %d)", x, y); // Use SDL_Log
}
int Player::getBombLimit() const {
    return bombLimit;
}

int Player::getX() const {
    return x;
}

int Player::getY() const{
    return y;
}