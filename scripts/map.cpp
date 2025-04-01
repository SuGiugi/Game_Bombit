#include "map.h"
#include "game.h"
#include <fstream>
#include <SDL.h> //Added for SDL_Log
#include <iostream>
#include "player.h"
#include "constant.h"
#include <cstring>
using namespace std;

Map::Map(const std::string& filename) {
    load(filename);
}

bool Map::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        SDL_Log("Failed to open map file: %s", filename.c_str());
        return false;
    }

    //Assuming a simple text-based map format (e.g., 0 for empty, 1 for wall)
    std::string line;
    while (std::getline(file, line)) {
        mapData.push_back(line);
    }

    width = static_cast<int>(mapData[0].length()); //All rows should be the same length
    height = static_cast<int>(mapData[0].size());
    SDL_Log("%d x %d", width, height);
    file.close();
    return true;
}

void Map::render(SDL_Renderer* renderer) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            SDL_Rect tileRect = {CENTER_X + x * TILE_SIZE,CENTER_Y +  y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            Resources::Instance()->render("grass",
                tileRect.x, tileRect.y,
                tileRect.w, tileRect.h,
                32, 32, renderer, SDL_FLIP_NONE);
            if (mapData[y][x] =='3') {
                //SDL_Rect rock = {7 , 0 , 52, 62};
                //SDL_RenderCopy(renderer,rockTexture, &rock, &tileRect);
                Resources::Instance()->render("rock",
                tileRect.x, tileRect.y,
                tileRect.w, tileRect.h,
                52, 62, renderer, SDL_FLIP_NONE);
            } else if (mapData[y][x] == '2') {
                // int ran = 1;
                // SDL_Rect tree = {65 * ran , 0 , 65, 80};
                // SDL_RenderCopy(renderer,objectTexture, &tree, &tileRect);
                Resources::Instance()->render("tree",
                tileRect.x, tileRect.y,
                tileRect.w, tileRect.h,
                65, 80, renderer, SDL_FLIP_NONE);
            } else if (mapData[y][x] == '4') {
                SDL_Rect rect = {24, 24, 140, 80};

                SDL_SetRenderDrawColor(renderer, 212, 201, 190, 255);
                SDL_RenderFillRect(renderer, &tileRect);
            }
        }
    }
}

char Map::limit(int x, int y) const {
    return mapData[y][x];
}

void Map::Create_map(char a, int x, int y) {
    int rate = 10;
    if (mapData[y][x] == '2') {
           rate = rand()%10;
    }
    mapData[y][x] = a;
    if (rate == 0) mapData[y][x] = '4';
}