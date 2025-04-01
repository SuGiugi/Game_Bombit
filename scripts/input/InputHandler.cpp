//
// Created by Admin on 3/31/2025.
//
#include "InputHandler.h"
#include "../game.h"

InputHandler::InputHandler() : mouseButtonDown(false), UpKeyDown(false), DownKeyDown(false), LeftKeyDown(false), RightKeyDown(false), SpaceKeyDown(false) {};

void InputHandler::handleEvents(bool& running, Player& player, Game& game, GAME_STATE& currentState, Map& map, std::vector<Bomb> &bombs) {
    SDL_Event event;
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    // Toggle pause state
                    if (currentState == GAME_STATE::PLAYING) {
                        currentState = GAME_STATE::PAUSE;
                    } else if (currentState == GAME_STATE::PAUSE) {
                        currentState = GAME_STATE::PLAYING;
                    }
                }
                else if (event.key.keysym.sym == SDLK_UP && !UpKeyDown) {
                    player.update(1, 0, -1, map);
                    UpKeyDown = true;
                }
                else if (event.key.keysym.sym == SDLK_DOWN && !DownKeyDown) {
                    player.update(0, 0, 1, map);
                    DownKeyDown = true;
                }
                else if (event.key.keysym.sym == SDLK_LEFT && !LeftKeyDown) {
                    player.update(2, -1, 0, map);
                    LeftKeyDown = true;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT && !RightKeyDown) {
                    player.update(3, 1, 0, map);
                    RightKeyDown = true;
                }
                else if (event.key.keysym.sym == SDLK_SPACE && !SpaceKeyDown) {
                    if (map.limit(player.getX(), player.getY()) == '0' && bombs.size() < player.getBombLimit()) {
                        map.Create_map('1', player.getX(), player.getY());
                        bombs.emplace_back(player.getX(), player.getY());
                    } else {
                        SDL_Log("cant place bomb");
                    }
                    SpaceKeyDown = true;
                }
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP) {
                    UpKeyDown = false;
                }
                else if (event.key.keysym.sym == SDLK_DOWN) {
                    DownKeyDown = false;
                }
                else if (event.key.keysym.sym == SDLK_LEFT) {
                    LeftKeyDown = false;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    RightKeyDown = false;
                }
                else if (event.key.keysym.sym == SDLK_SPACE) {
                    SpaceKeyDown = false;
                }
            default:
                break;


        }
    }
}