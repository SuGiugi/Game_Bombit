#include "explosion.h"
#include "../Render/resources.h"
#include "../Info/constant.h"
#include <../SDL2/SDL_image.h>

// Khoi tao vu no tai vi tri bom no
Explosion::Explosion(int x, int y, SDL_Renderer* renderer) :
    x(x), y(y), timer(10), finished(false), currentFrame(0) {

    //Set up the explosion rectangle array to define source rectangles from a sprite sheet
    for (int i = 0; i < NUM_FRAMES; i++) {
        explosionRects[i].x = i * TILE_SIZE; //Assuming each frame is TILE_SIZE wide
        explosionRects[i].y = 0; //Assuming single row of animation frames.
        explosionRects[i].w = TILE_SIZE;
        explosionRects[i].h = TILE_SIZE;
    }
}

// update Frame cua vu no
void Explosion::update() {
    timer--;
    if (timer <= 0) {
        finished = true;
    }
    else {
        currentFrame = (60 - timer) / (60 / NUM_FRAMES); //Progresses from 0-4 as time counts down.
        if (currentFrame >= NUM_FRAMES) currentFrame = NUM_FRAMES - 1; //Clamp
    }
}

// Dua vao frame de ve vu no
void Explosion::render(SDL_Renderer* renderer,string id, int px, int py) {
    if (id.empty()) return; //Don't try to render if texture failed to load.
    SDL_Rect destRect; //Where to put the explosion frame on screen.
    explosion_x = x + px;
    explosion_y = y + py;
    destRect.x = explosion_x * TILE_SIZE + CENTER_X;
    destRect.y = explosion_y * TILE_SIZE + CENTER_Y;
    destRect.w = TILE_SIZE;
    destRect.h = TILE_SIZE;

    Resources::Instance()->render(id,
        destRect.x, destRect.y,
        destRect.w, destRect.h,
        48, 48, renderer,SDL_FLIP_NONE);

}