#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "player.h"
#include "../scripts/object/bomb.h"
#include "map.h"
#include "../scripts/object/explosion.h"
#include "logic.h"
#include "../scripts/Enemy/enemy.h"
#include "../scripts/input/InputHandler.h"
#include <memory> //For smart pointers

//Constants (shared between source files)

enum class GAME_STATE {
    MAIN_MENU,
    PLAYING,
    PAUSE,
    GAME_OVER
};

class Game {
public:
    Game();
    bool init();
    void run();
    void handleInput(SDL_Event& event);
    void update();
    void render();
    void renderScore();
    void cleanup();
    void placeBomb(int x, int y);


    std::pair<int, int> position[4] = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(0, -1),
        std::make_pair(-1, 0)
    };
private:

    bool loadAssets();

    void handleEvent();
    void handleGameOverEvents();
    void handleMainMenuEvents();
    void handlePauseMenuEvents();
    void renderGameOver();

    SDL_Window* window;
    SDL_Renderer* renderer;
    InputHandler inputHandler;

    SDL_Texture* GameOverTexture;
    string explosionTexture[5];
    string background_Texture;

    TTF_Font* scoreFont;
    TTF_Font* menuFont;


    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color scoreColor = {255, 240, 133, 255};

    GAME_STATE current_state = GAME_STATE::PLAYING;

    Logic logic;
    Player player;  //Player object
    Map map; //The Map object.
    std::vector<Bomb> bombs; //Vector to store bomb objects
    std::vector<Explosion> explosions; // Store active explosions
    std::vector<Enemy> enemies;

    int score = 0;
    bool isRunning;
    int mouseX, mouseY;
};

#endif