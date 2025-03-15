#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <string>
#include <vector>
#include "player.h"

//Use the TILE_SIZE already defined in game.h

class Map {
public:
    Map(const std::string& filename);
    bool load(const std::string& filename);
    bool limit(int x, int y);
    void render(SDL_Renderer* renderer);

private:
    std::vector<std::string> mapData;
    int width;
    int height;
};

#endif