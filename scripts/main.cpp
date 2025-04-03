#include <SDL.h>
#include "game.h"
#include <memory>  // For smart pointers

int main(int argc, char* argv[]) {
    std::unique_ptr<Game> game = std::make_unique<Game>();
    // khoi tao game
    if (!game->init()) {
        SDL_Log("Game initialization failed. See previous SDL_Log messages.");
        return 1;
    }

    SDL_Log("Initialization complete. Starting game loop.");
    //chay game
    game->run();
    //clear game
    game->cleanup();

    SDL_Log("Game exited cleanly.");
    return 0;
}