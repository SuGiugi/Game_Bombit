#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <string>
#include <vector>
#include "player.h"
#include "../scripts/render/resources.h"

//Use the TILE_SIZE already defined in game.h

class Map {
public:
    Map(const std::string& filename);
    bool load(const std::string& filename);
    char limit(int x, int y);
    void render(SDL_Renderer* renderer, SDL_Texture* mapTexture);
    void Create_map(char a, int x, int y);
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    std::vector<std::string> mapData;
    int width;
    int height;

};

#endif