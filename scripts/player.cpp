
#include "player.h"

#include <chrono>
#include <SDL.h>  // Required for SDL_Log

#include "constant.h"

Player::Player(float startX, float startY) : x(startX), y(startY), speed(SPEED_PLAYER), bombLimit(2), direct(0) {}

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

float Player::get_last_x() const {
    return last_x;
}

float Player::get_last_y() const {
    return last_y;
}

