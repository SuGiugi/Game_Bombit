#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "player.h"
#include "bomb.h"
#include "map.h"
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

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    SDL_Texture* playerTexture; //Texture for Player (owned here)
    SDL_Texture* bombTexture; //Texture for bombs (owned here)

    Player player;  //Player object
    Map map; //The Map object.
    std::vector<Bomb> bombs; //Vector to store bomb objects
};

#endif