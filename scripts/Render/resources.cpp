#include "resources.h"
#include <SDL_image.h>
#include <SDL.h>
#include <bits/stdc++.h>
using namespace std;

Resources* Resources::instance = nullptr;

// Khoi tao hinh anh (img)
bool Resources::init() {
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_Log("Failed to initialize SDL_Image");
        return false;
    }
    SDL_Log("Successfully initialized SDL_Image");
    return true;
}

// load hinh anh va luu vao theo id
bool Resources::load(const string &fileName, const string &id, SDL_Renderer *renderer) {
    SDL_Surface* tempSurface = IMG_Load(fileName.c_str());

    if (!tempSurface) {
        SDL_Log("Failed to load image: %s", fileName.c_str());
        SDL_Log("SDL_Image Error", SDL_GetError());
        return false;
    } else {
        SDL_Log("Successfully loaded image: %s", fileName.c_str());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!texture) {
        SDL_Log("Failed to create texture: %s", fileName.c_str());
        SDL_Log("SDL Error", SDL_GetError());
        return false;
    }

    textureMap[id] = texture;
    return true;
}

// render hinh anh
void Resources::render(const string &id, int x, int y, int displayWidth, int displayHeight,
                             int frameWidth, int frameHeight,
                          SDL_Renderer *renderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect.x = 0;
    srcRect.y = 0;
    destRect.w = displayWidth;
    destRect.h = displayHeight;
    srcRect.w = frameWidth;
    srcRect.h = frameHeight;
    destRect.x = x;
    destRect.y = y;

    SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &destRect, 0, nullptr, flip);
}

// render hinh anh co frame
void Resources::renderFrame(const string& id, int x, int y, int displayWidth, int displayHeight,
                             int currentRow, int currentFrame, int frameWidth, int frameHeight,
                             SDL_Renderer* renderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect.x = frameWidth * currentFrame;
    srcRect.y = frameHeight * currentRow;
    srcRect.w = frameWidth;
    srcRect.h = frameHeight;

    destRect.x = x;
    destRect.y = y;
    destRect.w = displayWidth;
    destRect.h = displayHeight;

    SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &destRect, 0, nullptr, flip);
}

// xoa cac texture co trong map
void Resources::clearFromTextureMap(const string& id) {
    if (textureMap.find(id) != textureMap.end()) {
        SDL_DestroyTexture(textureMap[id]);
        textureMap.erase(id);
    }
}

void Resources::clean() {
    for (auto& texture : textureMap) {
        SDL_DestroyTexture(texture.second);
    }
    textureMap.clear();
    IMG_Quit();
}
