#ifndef RESOURCES_H
#define RESOURCES_H

#include <SDL.h>
#include <bits/stdc++.h>
using namespace std;

class Resources {
public:
    static Resources* Instance() {
        if (instance == nullptr) {
            instance = new Resources();
        }
        return instance;
    }
    bool init();
    bool load(const string& fileName, const string& id, SDL_Renderer* renderer);
    void render(const string &id, int x, int y, int displayWidth, int displayHeight, int frameWidth, int frameHeight,
                          SDL_Renderer *renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void renderFrame(const string& id, int x, int y, int displayWidth, int displayHeight,
                             int currentRow, int currentFrame, int frameWidth, int frameHeight,
                             SDL_Renderer* renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void clearFromTextureMap(const string& id);
    void clean();
    SDL_Texture* getTexture(const string& id) {
        return textureMap[id];
    }
private:
    Resources() {}
    ~Resources() {}

    static Resources* instance;
    map<string, SDL_Texture*> textureMap;
};
#endif