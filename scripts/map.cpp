#include "map.h"
#include "game.h"
#include <fstream>
#include <SDL.h> //Added for SDL_Log
#include "player.h"
#include <cstring>
using namespace std;

Map::Map(const string& filename) {
    if (!load(filename)) SDL_Log("Error loading map: %s", filename.c_str());
}

bool Map::load(const string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        SDL_Log("Failed to open map file: %s", filename.c_str());
        return false;
    }
    mapData.clear();
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

void Map::render(SDL_Renderer* renderer, int id) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            SDL_Rect tileRect = {CENTER_X + x * TILE_SIZE,CENTER_Y +  y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            Resources::Instance()->render("grass" + to_string(id),
                tileRect.x, tileRect.y,
                tileRect.w, tileRect.h,
                32, 32, renderer, SDL_FLIP_NONE);
            if (mapData[y][x] =='3') {
                Resources::Instance()->renderFrame("rock",
                tileRect.x, tileRect.y,
                tileRect.w, tileRect.h,
                0, id - 1,
                66, 64, renderer, SDL_FLIP_NONE);
            } else if (mapData[y][x] == '2') {
                Resources::Instance()->renderFrame("tree",
                tileRect.x, tileRect.y,
                tileRect.w, tileRect.h,
                0,id - 1,
                65, 80, renderer, SDL_FLIP_NONE);
            } else if ('4' <= mapData[y][x] && mapData[y][x] <= '7') {
                string c;
                switch (mapData[y][x]) {
                    case '4':  c = "Item4";
                    break;
                    case '5': c = "Item5";
                    break;
                    case '6': c = "Item6";
                    break;
                    case '7': c = "Item7";
                    break;
                    default: break;
                }
                Resources::Instance()->render(c,
                                tileRect.x, tileRect.y,
                                tileRect.w, tileRect.h,
                                64, 64, renderer, SDL_FLIP_NONE);
            } else if (mapData[y][x] == '8') {
                SDL_SetRenderDrawColor(renderer, 198, 142, 253, 255);
                SDL_RenderFillRect(renderer, &tileRect);
            } else if (mapData[y][x] == '9') {
                Resources::Instance()->renderFrame("shrub",
               tileRect.x, tileRect.y,
               tileRect.w, tileRect.h,
               0,3 - id,
               48, 48, renderer, SDL_FLIP_NONE);
            }
        }
    }
}

char Map::limit(int x, int y) const {
    if (mapData[y][x] == '9') {
        return '2';
    }
    return mapData[y][x];
}

void Map::Create_map(char a, int x, int y) {
    if (mapData[y][x] == '2' || mapData[y][x] == '9') {
           a = generate_item();
    }
    mapData[y][x] = a;
}

char Map::generate_item() {
    char a = '0';
    int rate = rand()%40;;
    if (rate == 0) a = '4';
    if (rate == 1) a = '5';
    if (rate == 2) a = '6';
    if (rate == 3) a = '7';
    return a;
}