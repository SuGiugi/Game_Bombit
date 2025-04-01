#include "game.h"
#include "player.h"
#include "../scripts/object/bomb.h"
#include "map.h"
#include "../scripts/render/resources.h"
#include "constant.h"
#include "../scripts/object/explosion.h"
#include "../scripts/Enemy/enemy.h"
#include <cmath>
#include <algorithm>
#include <cstring>

Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), player(8, 6), map("assets/maps/level1.txt"){}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log( "SDL could not initialize! SDL_Error: %s", SDL_GetError() );
        return false;
    } else SDL_Log("SDL Initialized Successful");

    if (TTF_Init() == -1) {
        SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s", TTF_GetError());
        return false;
    } else {
        SDL_Log("SDL_ttf Initialized Successful");
    }


    this->window = SDL_CreateWindow("Bombit!", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!this->window) {
        SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError() );
        SDL_Quit();
        return false;
    }

    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |  SDL_RENDERER_PRESENTVSYNC);
    if (!this->renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError() );
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    isRunning = true;

    // add enemies
    enemies.emplace_back(9,6, renderer);
    enemies.emplace_back(1,1, renderer);


    scoreFont = TTF_OpenFont("assets/font/Karma Suture.otf", 24);
    menuFont = TTF_OpenFont("assets/font/arial.ttf", 36);
     if (!scoreFont || !menuFont) {
        SDL_Log("Failed to load font! SDL_ttf Error: %s", TTF_GetError());
         return false;
     }


    if (!Resources::Instance()->init()) {
        SDL_Log("Resources could not be initialized");
        return false;
    } else {
        SDL_Log("Resources initialized Successful");
    }

    if (!loadAssets()) {
        return false;
    }
    return true;

}

bool Game::loadAssets() {
    //load player
    if (!player.loadTexture("assets/images/player/Slime1_Idle.png", "player", renderer)) {
        SDL_Log("Failed to load player texture");
        return false;
    };
    if (!player.loadTexture("assets/images/player/Slime1_Walk.png", "player_walk", renderer)) {
        SDL_Log("Failed to load player walk texture");
        return false;
    }
    if (!player.loadTexture("assets/images/player/heart.png", "heart", renderer)) {
        SDL_Log("Failed to load heart texture");
        return false;
    }
    //load enemy
    if (!Resources::Instance()->load("assets/images/enemies/Slime3_Idle.png", "enemy", renderer)) {
        SDL_Log("Failed to load enemyTexture");
        return false;
    }
    if (!Resources::Instance()->load("assets/images/enemies/Slime3_Attack.png", "enemy_attack", renderer)) {
        SDL_Log("Failed to load enemyTexture");
        return false;
    }
    if (!Resources::Instance()->load("assets/images/enemies/Slime3_Walk.png", "enemy_walk", renderer)) {
        SDL_Log("Failed to load enemyTexture");
        return false;
    }
    if (!Resources::Instance()->load("assets/images/enemies/Slime3_Death.png", "enemy_death", renderer)) {
        SDL_Log("Failed to load enemyTexture");
        return false;
    }
    if (!Resources::Instance()->load("assets/images/enemies/Slime3_Effect.png", "enemy_effect", renderer)) {
        SDL_Log("Failed to load enemyTexture");
        return false;
    }
    // load background
    if (!Resources::Instance()->load("assets/images/background/origbig.png", "background", renderer)) {
        SDL_Log("Failed to load background texture.");
        return false;
    }
    background_Texture = "background";
    //load bomb
    if (!Resources::Instance()->load("assets/images/bomb.png", "bomb", renderer)) {
        SDL_Log("Failed to load bomb");
        return false;
    }
    if (!Resources::Instance()->load("assets/images/grass.png", "grass", renderer)) {
        SDL_Log("Failed to load map texture.");
        return false;
    }
    if (!Resources::Instance()->load("assets/images/tree2.png", "tree", renderer)) {
        SDL_Log("Failed to load object texture.");
        return false;
    }
    if (!Resources::Instance()->load("assets/images/rock.png", "rock", renderer)) {
        SDL_Log("Failed to load rock texture.");
        return false;
    }
    //Load explosion;
    for (int i = 0; i < NUM_FRAMES; i++) {
        string c = "assets/images/explosion/" + to_string(i + 1) +".png";
        string id = "explosion" + to_string(i + 1);
        if (!Resources::Instance()->load(c, id, renderer)) {
            SDL_Log("Failed to load explosion");
            return false;
        }
        explosionTexture[i] = id;
    }

    return true;
}


void Game::renderGameOver() {

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 1);
    SDL_RenderClear(renderer);


    if (!(GameOverTexture == nullptr)) {
        int buttonWidth = 80;
        int buttonHeight = 83;
        SDL_Rect buttonRect = {
            (SCREEN_WIDTH - buttonWidth) / 2,
            (SCREEN_HEIGHT - buttonHeight) / 2,
            buttonWidth,
            buttonHeight
        };

        SDL_RenderCopyEx(renderer, GameOverTexture, &buttonRect, &buttonRect, 0, nullptr, SDL_FLIP_NONE);
    } else {
        SDL_Log("Unable to load restart texture! SDL Error: %s", SDL_GetError());
    }

    SDL_RenderPresent(renderer);
}

void Game::handleGameOverEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
            break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    int buttonWidth = 80;
                    int buttonHeight = 83;
                    SDL_Rect buttonRect = {
                        (SCREEN_WIDTH - buttonWidth) / 2,
                        (SCREEN_HEIGHT - buttonHeight) / 2,
                        buttonWidth,
                        buttonHeight
                    };

                    if (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
                        mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h) {
                        // Start game
                        current_state = GAME_STATE::PLAYING;
                        //resetGame();
                        }
                }
            break;
        }
    }
}

void Game::handleMainMenuEvents() {}
void Game::handlePauseMenuEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    current_state = GAME_STATE::PLAYING;
                }
            break;
        }
    }
}

void Game::renderScore() {
    std::string scoreText = "COLLECT: " + std::to_string(score) +"/3";


    SDL_Surface* scoreSurface = TTF_RenderText_Solid(scoreFont, scoreText.c_str(), scoreColor);
    if (!scoreSurface) {
        SDL_Log("Unable to render score text surface! SDL_ttf Error: %s", TTF_GetError());
        return;
    }

    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    if (!scoreTexture) {
        SDL_Log("Unable to create score texture! SDL Error: %s", SDL_GetError());
        SDL_FreeSurface(scoreSurface);
        return;
    }

    int textWidth = scoreSurface->w;
    int textHeight = scoreSurface->h;

    SDL_Rect renderQuad = {
        SCREEN_WIDTH - textWidth - 15,
        20,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, scoreTexture, nullptr, &renderQuad);

    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
}

// Game run loop
void Game::run() {
    SDL_Event event;

    while (isRunning) {
        handleEvent();
        update();
        render();
        SDL_Delay(18);
    }
}

void Game::handleEvent() {
    SDL_GetMouseState(&mouseX, &mouseY);

    switch (current_state) {
        case GAME_STATE::PLAYING:
            this->inputHandler.handleEvents(this->isRunning, this->player, *this, current_state, map, bombs);
        break;

        case GAME_STATE::MAIN_MENU:
            handleMainMenuEvents();
        break;

        case GAME_STATE::PAUSE:
            handlePauseMenuEvents();
        break;

        case GAME_STATE::GAME_OVER:
            handleGameOverEvents();
        break;
        default:
            break;
    }
}


void Game::update() {
    switch (current_state) {
        case GAME_STATE::PLAYING:
            for (auto it = bombs.begin(); it != bombs.end();) {
                it->update();
                if (it->isExploded()) {
                    SDL_Log("Bomb exploded at (%d, %d)", it->getX(), it->getY());

                    map.Create_map('0', it->getX(), it->getY());

                    explosions.emplace_back(it->getX(), it->getY(), renderer); //Example radius of 2
                    it = bombs.erase(it);

                }
                else {
                    ++it;
                }
            }

        //Update Explosions
        for (auto it = explosions.begin(); it != explosions.end();) {
            if (it->isFinished()) {
                it = explosions.erase(it);
            }
            else {
                ++it;
            }
        }
        for (auto it = enemies.begin(); it != enemies.end();) {
            it->update(static_cast<int> (logic.round_2(player.getX())),static_cast<int> (logic.round_2(player.getY())), map); //Use it pointer access
            if (it ->get_skill() == true) {
                for (auto u : position) {
                    int dx = it->getX() + u.first;
                    int dy = it->getY() + u.second;
                    if (map.limit(dx, dy) == '2') map.Create_map('0', dx, dy);
                }
            }
            if (it->isDeath() == 3) {
                score++;
                it = enemies.erase(it);
            }
            else ++it;
        }
        break;
        case GAME_STATE::MAIN_MENU:
            handleMainMenuEvents();
            break;
        case GAME_STATE::PAUSE:
            handlePauseMenuEvents();
            break;
        case GAME_STATE::GAME_OVER:
            handleGameOverEvents();
            break;
        default:
            break;
    }


}

void Game::render() {
    SDL_RenderClear(renderer);
    SDL_Rect dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    if (!(background_Texture.empty())) {
        Resources::Instance()->render(background_Texture,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 2304, 1296, renderer, flip );
    }
    renderScore();
    //Render the Map:
    map.render(renderer);
    // Draw Player:
    player.render_player(renderer);

    //Draw Bombs:
    for (const auto& bomb : bombs) {
        SDL_Rect bombRect = {CENTER_X + bomb.getX() * TILE_SIZE + 3, CENTER_Y + bomb.getY() * TILE_SIZE + 3, TILE_SIZE - 6, TILE_SIZE - 6};
        Resources::Instance()->render("bomb",
            bombRect.x, bombRect.y,
            bombRect.w, bombRect.h,
            500, 500, renderer, flip);
        //SDL_RenderCopy(renderer, bombTexture, NULL, &bombRect);
    }
    for (auto& explosion : explosions) {
        explosion.update();
        int current_explosion_X = explosion.get_X();
        int current_explosion_Y = explosion.get_Y();
        for (auto it = enemies.begin(); it != enemies.end();) {
            double enemy_x = it->getX();
            double enemy_y = it->getY();
            if ((current_explosion_X - logic.round_2(enemy_x)) * (current_explosion_X - logic.round_2(enemy_x)) + (current_explosion_Y - logic.round_2(enemy_y)) * (current_explosion_Y - logic.round_2(enemy_y))  < 1)
            {it -> Death();}
            ++it;
        }
        if ((current_explosion_X - logic.round_2(player.getX())) * (current_explosion_X - logic.round_2(player.getX())) + (current_explosion_Y - logic.round_2(player.getY())) * (current_explosion_Y - logic.round_2(player.getY()))  < 1 && explosion.get_hurt() == false) {
            player.hurt();
            explosion.is_hurt();
        }
        explosion.render(renderer,explosionTexture[0], 0,0);
        for (auto u : position) {
            for (int i = 1; i < player.size(); i++) {
                current_explosion_X = explosion.get_X() + u.first * i;
                current_explosion_Y = explosion.get_Y() + u.second * i;
                if (map.limit(current_explosion_X, current_explosion_Y) == '2' && explosion.isFinished() == true) {
                    map.Create_map('0', current_explosion_X, current_explosion_Y);
                    break;
                } else if (map.limit(current_explosion_X, current_explosion_Y) > '1') {
                    break;
                }
                if ((current_explosion_X - logic.round_2(player.getX())) * (current_explosion_X - logic.round_2(player.getX())) + (current_explosion_Y - logic.round_2(player.getY())) * (current_explosion_Y - logic.round_2(player.getY()))  < 1 && explosion.get_hurt() == false) {
                    player.hurt();
                    explosion.is_hurt();
                }
                explosion.render(renderer,explosionTexture[i], u.first*i,u.second*i);
                for (auto it = enemies.begin(); it != enemies.end();) {
                    double enemy_x = it->getX();
                    double enemy_y = it->getY();
                    if ((current_explosion_X - logic.round_2(enemy_x)) * (current_explosion_X - logic.round_2(enemy_x)) + (current_explosion_Y - logic.round_2(enemy_y)) * (current_explosion_Y - logic.round_2(enemy_y))  < 1) {it->Death();};
                    ++it;
                }
            }

        }
    }

    for (auto& enemy : enemies) {
        enemy.render(renderer, player.getX(),player.getY());
        if (enemy.is_kill() && !enemy.is_hurt()) {
            player.hurt();
            enemy.hurt_player();
        }
    }
    switch (current_state) {
        case GAME_STATE::PLAYING:
            break;
        case GAME_STATE::PAUSE:
            break;
        default:
            break;

    }

    SDL_RenderPresent(renderer);
}




void Game::cleanup() {
    Resources::Instance()->clean();
    if (this->renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (this->window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}