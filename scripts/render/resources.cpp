#include "resources.h"
#include <SDL_image.h>
#include <SDL.h>

SDL_Texture* loadTexture(const char* filepath, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(filepath);
    if (surface == nullptr) {
        SDL_Log("Error loading image: %s. SDL_image Error: %s", filepath, IMG_GetError() );
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Free the surface immediately.

    if (texture == nullptr) {
        SDL_Log("Error creating texture from surface: %s", SDL_GetError() );
        return nullptr;
    }

    return texture;
}