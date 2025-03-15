#include "explosion.h"
#include <SDL.h> //Added for SDL_Log
// Add your explosion implementation here

void Explosion::update(int px, int py, int bx, int by, int size) { //px = play.x, py = play.y
    explode = false;
    if ((px == bx && by + size >=  py && by - size <= py) || (py == by && bx + size >= px && bx - size <= px)) explode = true;
}
bool Explosion::getExplode() const {
    return explode;
}