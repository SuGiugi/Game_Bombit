#include "game.h"
#include "player.h"
#include "bomb.h"
#include "map.h"
#include "resources.h"
#include "constant.h"
#include "explosion.h"
#include <cmath>
#include <algorithm>

Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), player(1, 1), map("assets/maps/level1.txt"), playerTexture(nullptr), bombTexture(nullptr) {}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log( "SDL could not initialize! SDL_Error: %s", SDL_GetError() );
        return false;
    }

    window = SDL_CreateWindow("Bombit!", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError() );
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError() );
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    isRunning = true;

    //Load the player texture
    playerTexture = loadTexture("assets/images/player.png", renderer);
    if (playerTexture == nullptr) {
        SDL_Log("Failed to load player texture.");
        return false;
    }
    //Load the bomb texture
    bombTexture = loadTexture("assets/images/bomb.png", renderer);
     if (bombTexture == nullptr) {
         SDL_Log("Failed to load bomb texture.");
        return false;
    }

    //Load the map texture
    mapTexture = loadTexture("assets/images/ground.png", renderer);
    if (mapTexture == nullptr) {
        SDL_Log("Failed to load player texture.");
        return false;
    }

    return true;
}

void Game::run() {
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            handleInput(event);
        }

        update();
        render();
        SDL_Delay(PRESS_DELAY);
    }
}

void Game::handleInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        float dx = 0.0f;
        float dy = 0.0f;
        int bomb_x = static_cast<int>(logic.round_2(player.getX()) + FIXED);
        int bomb_y = static_cast<int>(logic.round_2(player.getY()) + FIXED) ;


        switch (event.key.keysym.sym) {
            case SDLK_UP:
                dy = -1.0f;
                break;
            case SDLK_DOWN:
                dy = 1.0f;
                break;
            case SDLK_LEFT:
                dx = -1.0f;
                break;
            case SDLK_RIGHT:
                dx = 1.0f;
                break;
            case SDLK_SPACE:
                if (map.limit(bomb_x, bomb_y) == '0') {
                    map.Create_map('3',bomb_x, bomb_y);
                    placeBomb(bomb_x,bomb_y);
                    SDL_Log("%c", map.limit(bomb_x,bomb_y));
                } else {
                    SDL_Log("Cant place");
                }
                break;
            case SDLK_b:
                placeBomb(bomb_x,bomb_y);
                break;
        }

        float newX = player.getX() + dx * player.getSpeed();
        float newY = player.getY() + dy * player.getSpeed();
        int limit_x = 0;
        int limit_y = 0;
        //MoveY
        if (dy == -1.0f && logic.down(logic.round_2(player.getY()), 0) != logic.down(logic.round_2(newY), 0)) {
            limit_y = logic.down(newY, 0);
            if (!(map.limit(logic.up(logic.round_2(player.getX()),0), limit_y) == '0') || !(map.limit(logic.down(logic.round_2(player.getX()),0), limit_y) == '0')) {
                SDL_Log("Player up attempt failed");
                newY = player.getY();
            }
        }else if (dy == 1.0f && logic.up(logic.round_2(player.getY()), 0) != logic.up(logic.round_2(newY), 0)) {
            limit_y = logic.up(newY, 0);
            if (!(map.limit(logic.up(logic.round_2(player.getX()),0), limit_y) == '0') || !(map.limit(logic.down(logic.round_2(player.getX()),0), limit_y) == '0')) {
                SDL_Log("Player up attempt failed");
                newY = player.getY();
            }
        } else if (dx == -1.0f && logic.down(logic.round_2(player.getX()), 0) != logic.down(logic.round_2(newX), 0)) {
            limit_x = logic.down(newX, 0);
            if (!(map.limit(limit_x,logic.up(logic.round_2(player.getY()),0)) == '0') || !(map.limit(limit_x,logic.down(logic.round_2(player.getY()),0)) == '0')) {
                SDL_Log("Player up attempt failed");
                newX = player.getX();
            }
        }
        else if (dx == 1.0f && logic.up(logic.round_2(player.getX()), 0) != logic.up(logic.round_2(newX), 0)) {
            limit_x = logic.up(newX, 0);
            if (!(map.limit(limit_x,logic.up(logic.round_2(player.getY()),0)) == '0') || !(map.limit(limit_x,logic.down(logic.round_2(player.getY()),0)) == '0')) {
                SDL_Log("Player up attempt failed");
                newX = player.getX();
            }
        }
        SDL_Log("%f %f",newX, newY);
        player.setX(newX);
        player.setY(newY);

    }
}

void Game::update() {
    for (auto it = bombs.begin(); it != bombs.end();) {
        it->update();
        if (it->isExploded()) {
            SDL_Log("Bomb exploded at (%d, %d)", it->getX(), it->getY());

            //Create an explosion object here!
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
}

void Game::render() {
    SDL_RenderClear(renderer);

    //Render the Map:
    map.render(renderer, mapTexture);
    // Draw Player:
    SDL_Rect playerRect = { CENTER_X + static_cast<int>(player.getX() * TILE_SIZE),CENTER_Y + static_cast<int>(player.getY() * TILE_SIZE), TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

    //Draw Bombs:
    for (const auto& bomb : bombs) {
        SDL_Rect bombRect = {CENTER_X + bomb.getX() * TILE_SIZE, CENTER_Y + bomb.getY() * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, bombTexture, NULL, &bombRect);
    }

    for (auto& explosion : explosions) {
        explosion.update();
        int current_explosion_X = explosion.get_X();
        int current_explosion_Y = explosion.get_Y();
        if ((current_explosion_X ==  logic.up(logic.round_2(player.getX()), 0) || current_explosion_X ==  logic.down(logic.round_2(player.getX()), 0))  && ( current_explosion_Y == logic.up(logic.round_2(player.getY()), 0) || current_explosion_Y == logic.down(logic.round_2(player.getY()), 0)))
        {SDL_Log("Die");}
        explosion.render(renderer, 0,0);
        for (auto u : position) {
            for (int i = 1; i < 5; i++) {
                current_explosion_X = explosion.get_X() + u.first * i;
                current_explosion_Y = explosion.get_Y() + u.second * i;
                if (map.limit(current_explosion_X, current_explosion_Y) == '2' && explosion.isFinished() == true) {
                    map.Create_map('0', current_explosion_X, current_explosion_Y);
                    break;
                } else if (map.limit(current_explosion_X, current_explosion_Y) != '0') {
                    break;
                }

                explosion.render(renderer, u.first*i,u.second*i);
                if ((current_explosion_X ==  logic.up(logic.round_2(player.getX()), 0) || current_explosion_X ==  logic.down(logic.round_2(player.getX()), 0))  && ( current_explosion_Y == logic.up(logic.round_2(player.getY()), 0) || current_explosion_Y == logic.down(logic.round_2(player.getY()), 0)))
                {SDL_Log("Die");}
            }

        }
    }

    SDL_RenderPresent(renderer);
}

void Game::placeBomb(int x, int y) {
    if (bombs.size() < player.getBombLimit()) {
        bombs.emplace_back(x, y);
         SDL_Log("Bomb placed at (%d, %d)", x, y);
    } else {
        SDL_Log("Cannot place more bombs!");
    }
}


void Game::cleanup() {
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(bombTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}