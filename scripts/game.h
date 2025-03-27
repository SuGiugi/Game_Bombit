#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "player.h"
#include "bomb.h"
#include "map.h"
#include "explosion.h"
#include "logic.h"
#include "../scripts/Enemy/enemy.h"
#include <memory> //For smart pointers

//Constants (shared between source files)

class Game {
public:
    Game();
    bool init();
    void run();
    void handleInput(SDL_Event& event);
    void update();
    void render();
    void cleanup();
    void placeBomb(int x, int y);
    void move(const float &current_x,const float &current_y, float &next_x, float &next_y, double dx, double dy);

    std::pair<int, int> position[4] = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(0, -1),
        std::make_pair(-1, 0)
    };
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    SDL_Texture* playerTexture; //Texture for Player (owned here)
    SDL_Texture* bombTexture; //Texture for bombs (owned here)
    SDL_Texture* mapTexture; //Texture for map(owned here)

    Logic logic;
    Player player;  //Player object
    Map map; //The Map object.
    std::vector<Bomb> bombs; //Vector to store bomb objects
    std::vector<Explosion> explosions; // Store active explosions
    std::vector<Enemy> enemies;
};

#endif