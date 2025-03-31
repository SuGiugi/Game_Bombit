//
// Created by Admin on 3/30/2025.
//
#include "background.h"

#include <SDL_image.h>
#include <SDL.h>
#include <cstring>
#include "../constant.h"
#include "../render/resources.h"
void Background::load(SDL_Renderer *renderer) {
    std::string c = "assets/images/background/origbig.png";
    background = loadTexture(c.c_str(), renderer);
}

void Background::render(SDL_Renderer *renderer) {
    SDL_Rect dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer ,background, NULL, &dest);
}



