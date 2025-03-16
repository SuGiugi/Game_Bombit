
#include "player.h"
#include <SDL.h>  // Required for SDL_Log

#include "constant.h"

Player::Player(float startX, float startY) : x(startX), y(startY), speed(SPEED_PLAYER), bombLimit(1) {}

void Player::move(float dx, float dy) {
    float movementAmountX = speed * dx;
    float movementAmountY = speed * dy;
    x += movementAmountX;
    y += movementAmountY;
    SDL_Log("Player moved to (%d, %d)", x, y); // Use SDL_Log
}
int Player::getBombLimit() const {
    return bombLimit;
}

float Player::getX() const {
    return x;
}

float Player::getY() const{
    return y;
}