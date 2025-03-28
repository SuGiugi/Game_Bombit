#include "explosion.h"//For TILE_SIZE
#include "../scripts/render/resources.h"
#include "constant.h"
#include <SDL_image.h> //Texture Loading

Explosion::Explosion(int x, int y, SDL_Renderer* renderer) :
    x(x), y(y), timer(30), finished(false), texture(nullptr), currentFrame(0) {
    texture = loadTexture("assets/images/player.png", renderer);
    if (texture == nullptr) {
        SDL_Log("Failed to load explosion texture!");
        finished = true; //Don't try to render if we don't have a texture.
        return;
    }

    //Set up the explosion rectangle array to define source rectangles from a sprite sheet
    for (int i = 0; i < NUM_FRAMES; i++) {
        explosionRects[i].x = i * TILE_SIZE; //Assuming each frame is TILE_SIZE wide
        explosionRects[i].y = 0; //Assuming single row of animation frames.
        explosionRects[i].w = TILE_SIZE;
        explosionRects[i].h = TILE_SIZE;
    }
}

Explosion::~Explosion() {
    //SDL_DestroyTexture(texture);
}

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

void Explosion::render(SDL_Renderer* renderer, int px, int py) {
    if (texture == nullptr) return; //Don't try to render if texture failed to load.
    SDL_Rect destRect; //Where to put the explosion frame on screen.
    explosion_x = x + px;
    explosion_y = y + py;
    destRect.x = explosion_x * TILE_SIZE + CENTER_X;
    destRect.y = explosion_y * TILE_SIZE + CENTER_Y;
    destRect.w = TILE_SIZE;
    destRect.h = TILE_SIZE;

    SDL_RenderCopy(renderer, texture, NULL, &destRect);

}