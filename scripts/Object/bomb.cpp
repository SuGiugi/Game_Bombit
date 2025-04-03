//
// Created by Admin on 3/14/2025.
//
#include "bomb.h"
#include "../Info/constant.h"
#include <SDL.h>

// khoi tao bomb tai vi tri dat
Bomb::Bomb(int x, int y) : x(x), y(y), timer(TIME_EXPLODE), exploded(false) {}

// update thoi gian bom no
void Bomb::update() {
    // SDL_Log("%d", timer);
    if (timer > 0) {
        timer--;
    } else {
        exploded = true;
    }
}

bool Bomb::isExploded() const {
    return exploded;
}

int Bomb::getX() const { return x; }
int Bomb::getY() const { return y; }