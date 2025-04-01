//
// Created by Admin on 3/14/2025.
//
#include "bomb.h"
#include "../constant.h"
#include <SDL.h>

Bomb::Bomb(int x, int y) : x(x), y(y), timer(TIME_EXPLODE), exploded(false) {}

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