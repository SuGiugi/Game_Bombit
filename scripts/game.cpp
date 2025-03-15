#include "game.h"
#include "player.h"
#include "bomb.h"
#include "map.h"
#include "resources.h"
#include "constant.h"
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
        SDL_Delay(10);
    }
}

void Game::handleInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                if (map.limit( player.getX(), player.getY() - 1)) player.move(0, -1);
                else SDL_Log("Player cant moved up");
                break;
            case SDLK_DOWN:
                if (map.limit( player.getX(), player.getY() + 1)) player.move(0, 1);
                else SDL_Log("Player cant moved down");
                break;
            case SDLK_LEFT:
                if (map.limit( player.getX() - 1, player.getY())) player.move(-1, 0);
                else SDL_Log("Player cant moved left");
                break;
            case SDLK_RIGHT:
                if (map.limit( player.getX() + 1, player.getY())) player.move(1, 0);
                else SDL_Log("Player cant moved right");
                break;
            case SDLK_SPACE:
                placeBomb(player.getX(), player.getY());
                break;
            case SDLK_b:
                placeBomb(player.getX(), player.getY());
                break;
        }
    }
}

void Game::update() {
    for (auto it = bombs.begin(); it != bombs.end();) {
        it->update();
        if (it->isExploded()) {
            SDL_Log("Bomb exploded at (%d, %d)", it->getX(), it->getY());
            // TODO: implement explosion effects and damage
            it = bombs.erase(it);

        } else {
            ++it;
        }
    }
}

void Game::render() {
    SDL_RenderClear(renderer);

    //Render the Map:
    map.render(renderer);
    // Draw Player:
    SDL_Rect playerRect = {player.getX() * TILE_SIZE, player.getY() * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

    //Draw Bombs:
    for (const auto& bomb : bombs) {
        SDL_Rect bombRect = {bomb.getX() * TILE_SIZE, bomb.getY() * TILE_SIZE, TILE_SIZE, TILE_SIZE};
         SDL_RenderCopy(renderer, bombTexture, NULL, &bombRect);
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