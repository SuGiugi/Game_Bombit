#include "explosion.h"
#include "game.h" //For TILE_SIZE
#include "resources.h"
#include "constant.h"
#include <SDL_image.h> //Texture Loading

Explosion::Explosion(int x, int y, SDL_Renderer* renderer) :
    x(x), y(y), timer(60), finished(false), texture(nullptr), currentFrame(0) {
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

void Explosion::render(SDL_Renderer* renderer, float px, float py) {
    if (texture == nullptr) return; //Don't try to render if texture failed to load.
    SDL_Rect destRect; //Where to put the explosion frame on screen.
    for (auto u : position) {
        destRect.x = (x + currentFrame * u.first) * TILE_SIZE + CENTER_X;
        destRect.y = (y + currentFrame * u.second) * TILE_SIZE + CENTER_Y;
        destRect.w = TILE_SIZE;
        destRect.h = TILE_SIZE;
        SDL_Log("%d %d %d %d", x + currentFrame * u.first, y + currentFrame * u.second,px,  py  );
        if ((x + currentFrame * u.first ==  logic.up(px, 0) || x + currentFrame * u.first ==  logic.down(px, 0))  && ( y + currentFrame * u.second == logic.up(py, 0) || y + currentFrame * u.second == logic.down(py, 0))) {SDL_Log("Die");}
        SDL_RenderCopy(renderer, texture, &explosionRects[currentFrame], &destRect);
    }
}