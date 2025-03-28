#ifndef MAP_H
#define MAP_H

#include <map>
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
    char limit(int x, int y) const;
    void render(SDL_Renderer* renderer, SDL_Texture* mapTexture, SDL_Texture* objectTexture, SDL_Texture* rockTexture);
    void Create_map(char a, int x, int y);
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    std::vector<std::string> get_map() { return mapData; }

private:
    std::vector<std::string> mapData;
    int width;
    int height;

};

#endif