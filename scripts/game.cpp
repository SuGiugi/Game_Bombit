#include "game.h"
#include "player.h"
#include "bomb.h"
#include "map.h"
#include "../scripts/render/resources.h"
#include "constant.h"
#include "explosion.h"
#include "../scripts/Enemy/enemy.h"
#include <cmath>
#include <algorithm>

Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), player(8, 6), map("assets/maps/level1.txt"), playerTexture(nullptr), bombTexture(nullptr), walk(false){}

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
    playerTexture = loadTexture("assets/images/player/Slime1_Idle.png", renderer);
    player_walk_Texture = loadTexture("assets/images/player/Slime1_Walk.png", renderer);
    if (playerTexture == nullptr || player_walk_Texture == nullptr) {
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
    mapTexture = loadTexture("assets/images/grass.png", renderer);
    if (mapTexture == nullptr) {
        SDL_Log("Failed to load map texture.");
        return false;
    }

    objectTexture = loadTexture("assets/images/tree2.png", renderer);
    if (objectTexture == nullptr) {
        SDL_Log("Failed to load object texture.");
        return false;
    }
    rockTexture = loadTexture("assets/images/rock.png", renderer);
    if (rockTexture == nullptr) {
        SDL_Log("Failed to load rock texture.");
        return false;
    }

    enemies.emplace_back(9,6, renderer);
    enemies.emplace_back(1,1, renderer);
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
        SDL_Delay(30);
    }
}

void Game::handleInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        float dx = 0.0f;
        float dy = 0.0f;
        int bomb_x = static_cast<int>(logic.round_2(player.getX()) + FIXED);
        int bomb_y = static_cast<int>(logic.round_2(player.getY()) + FIXED) ;


        switch (event.key.keysym.sym) {
            case SDLK_UP:
                dy = -1.0f;
                if (!walk) player.set_direct(1);
                break;
            case SDLK_DOWN:
                dy = 1.0f;
                if (!walk) player.set_direct(0);
                break;
            case SDLK_LEFT:
                dx = -1.0f;
                if (!walk) player.set_direct(2);
                break;
            case SDLK_RIGHT:
                dx = 1.0f;
                if (!walk) player.set_direct(3);
                break;
            case SDLK_SPACE:
                if (map.limit(bomb_x, bomb_y) == '0') {
                    placeBomb(bomb_x,bomb_y);
                } else {
                    SDL_Log("Cant place");
                }
                break;
            case SDLK_b:
                placeBomb(bomb_x,bomb_y);
                break;
        }
        if (!walk) {
            float newX = player.getX() + dx * player.getSpeed();
            float newY = player.getY() + dy * player.getSpeed();
            player.set_last_xy(player.getX(), player.getY());
            move(player.getX(), player.getY(), newX, newY, dx, dy);
            if ((player.get_last_x() != newX || player.get_last_y() != newY)) {
                SDL_Log("walk");
                walk = true;
                player.set_time();
                player.setXY(newX, newY);
            }
        }

    }
}

void Game::update() {
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
        if (it->isDeath() == 3) it = enemies.erase(it);
        else ++it;
    }
}

void Game::render() {
    SDL_RenderClear(renderer);

    //Render the Map:
    map.render(renderer, mapTexture, objectTexture, rockTexture);
    // Draw Player:
    player.render_player(renderer, playerTexture, player_walk_Texture, walk);

    //Draw Bombs:
    for (const auto& bomb : bombs) {
        SDL_Rect bombRect = {CENTER_X + bomb.getX() * TILE_SIZE + 3, CENTER_Y + bomb.getY() * TILE_SIZE + 3, TILE_SIZE - 6, TILE_SIZE - 6};
        SDL_RenderCopy(renderer, bombTexture, NULL, &bombRect);
    }

    for (auto& explosion : explosions) {
        explosion.update();
        int current_explosion_X = explosion.get_X();
        int current_explosion_Y = explosion.get_Y();
        if ((current_explosion_X - logic.round_2(player.getX())) * (current_explosion_X - logic.round_2(player.getX())) + (current_explosion_Y - logic.round_2(player.getY())) * (current_explosion_Y - logic.round_2(player.getY()))  < 1)
        {SDL_Log("%d", (current_explosion_X - logic.round_2(player.getX())) * (current_explosion_X - logic.round_2(player.getX())) + (current_explosion_Y - logic.round_2(player.getY())) * (current_explosion_Y - logic.round_2(player.getY())));}
        for (auto it = enemies.begin(); it != enemies.end();) {
            double enemy_x = it->getX();
            double enemy_y = it->getY();
            if ((current_explosion_X - logic.round_2(enemy_x)) * (current_explosion_X - logic.round_2(enemy_x)) + (current_explosion_Y - logic.round_2(enemy_y)) * (current_explosion_Y - logic.round_2(enemy_y))  < 1)
            {it -> Death();}
            ++it;
        }
        explosion.render(renderer, 0,0);
        for (auto u : position) {
            for (int i = 1; i < SIZE_EXPLODE; i++) {
                current_explosion_X = explosion.get_X() + u.first * i;
                current_explosion_Y = explosion.get_Y() + u.second * i;
                if (map.limit(current_explosion_X, current_explosion_Y) == '2' && explosion.isFinished() == true) {
                    map.Create_map('0', current_explosion_X, current_explosion_Y);
                    break;
                } else if (map.limit(current_explosion_X, current_explosion_Y) != '0') {
                    break;
                }
                explosion.render(renderer, u.first*i,u.second*i);
                if ((current_explosion_X - logic.round_2(player.getX())) * (current_explosion_X - logic.round_2(player.getX())) + (current_explosion_Y - logic.round_2(player.getY())) * (current_explosion_Y - logic.round_2(player.getY()))  < 1)
                {SDL_Log("%d %f %f %f",current_explosion_Y,player.getY() ,(current_explosion_X - logic.round_2(player.getX()) + u.first*i) * (current_explosion_X - logic.round_2(player.getX()) + u.first*i), current_explosion_Y - logic.round_2(player.getY()));}
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
        enemy.render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::placeBomb(int x, int y) {
    if (bombs.size() < player.getBombLimit()) {
        map.Create_map('1',x, y);
        bombs.emplace_back(x, y);
         SDL_Log("Bomb placed at (%d, %d)", x, y);
    } else {
        SDL_Log("Cannot place more bombs!");
    }
}

void Game::move(const float &current_x, const float &current_y, float &next_x, float &next_y, double dx, double dy) {
    int limit_x = 0;
    int limit_y = 0;
    //MoveY
    if (dy < 0.0f && logic.down(logic.round_2(current_y), 0) != logic.down(logic.round_2(next_y), 0)) {
        limit_y = logic.down(next_y, 0);
        if (!(map.limit(logic.up(logic.round_2(current_x),0), limit_y) <= '1') || !(map.limit(logic.down(logic.round_2(current_x),0), limit_y) <= '1')) {
            SDL_Log("Player up attempt failed");
            next_y = current_y;
        }
    }else if (dy > 0.0f && logic.up(logic.round_2(current_y), 0) != logic.up(logic.round_2(next_y), 0)) {
        limit_y = logic.up(next_y , 0);
        if (!(map.limit(logic.up(logic.round_2(current_x),0), limit_y) <= '1') || !(map.limit(logic.down(logic.round_2(current_x),0), limit_y) <= '1')) {
            SDL_Log("Player up attempt failed");
            next_y  = current_y;
        }
    } else if (dx < 0.0f && logic.down(logic.round_2(current_x), 0) != logic.down(logic.round_2(next_x), 0)) {
        limit_x = logic.down(next_x, 0);
        if (!(map.limit(limit_x,logic.up(logic.round_2(current_y),0)) <= '1') || !(map.limit(limit_x,logic.down(logic.round_2(current_y),0)) <= '1')) {
            SDL_Log("Player up attempt failed");
            next_x = current_x;
        }
    }
    else if (dx > 0.0f && logic.up(logic.round_2(current_x), 0) != logic.up(logic.round_2(next_x), 0)) {
        limit_x = logic.up(next_x, 0);
        if (!(map.limit(limit_x,logic.up(logic.round_2(current_y),0)) <= '1') || !(map.limit(limit_x,logic.down(logic.round_2(current_y),0)) <= '1')) {
            SDL_Log("Player up attempt failed");
            next_x = current_x;
        }
    }
}


void Game::cleanup() {
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(bombTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}