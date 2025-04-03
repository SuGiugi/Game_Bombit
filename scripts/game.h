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
    void renderScore();
    void cleanup();


    std::pair<int, int> position[4] = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(0, -1),
        std::make_pair(-1, 0)
    };
private:

    bool loadAssets();
    void write_status(SDL_Renderer* renderer,const int &number,const int &size) const;
    void resetGame();
    void update();
    void render();
    void renderTutorial(const int &id);
    void renderGameOver();
    void renderPause();
    void renderMainMenu();
    void handleGameTutorial();
    void handleGameOverEvents();
    void handleMainMenuEvents();
    void handlePauseMenuEvents();

    SDL_Window* window;
    SDL_Renderer* renderer;
    InputHandler inputHandler;

    SDL_Texture* GameOverTexture;
    string explosionTexture[6];

    TTF_Font* Font;


    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Color textColor = {221, 168, 83, 255};
    SDL_Color Color = {255, 240, 133, 255};

    GAME_STATE current_state = GAME_STATE::MAIN_MENU;

    Logic logic;
    Player* player;  //Player object
    Map map; //The Map object.
    std::vector<Bomb> bombs; //Vector to store bomb objects
    std::vector<Explosion> explosions; // Store active explosions
    std::vector<Enemy> enemies;

    int background_Texture = 2;
    int Menu_id = 0;
    int time_EnemySpawn = 0;
    int cnt_tutorial = 0;
    int score = 0;
    bool tutorial = false;
    bool isRunning = false;
};

#endif