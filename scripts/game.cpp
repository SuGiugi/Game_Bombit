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

Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), map("assets/maps/level1.txt"), player(nullptr){}

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



    scoreFont = TTF_OpenFont("assets/font/Karma Suture.otf", 24);
     if (!scoreFont) {
        SDL_Log("Failed to load font! SDL_ttf Error: %s", TTF_GetError());
         return false;
     }


    if (!Resources::Instance()->init()) {
        SDL_Log("Resources could not be initialized");
        return false;
    } else {
        SDL_Log("Resources initialized Successful");
    }

    player = new Player(6, 6);

    if (!loadAssets()) {
        return false;
    }
    return true;

}

bool Game::loadAssets() {
    //load player
    if (!player->loadTexture("assets/images/player/Slime1_Idle.png", "player", renderer)) {
        SDL_Log("Failed to load player texture");
        return false;
    };
    if (!player->loadTexture("assets/images/player/Slime1_Walk.png", "player_walk", renderer)) {
        SDL_Log("Failed to load player walk texture");
        return false;
    }
    if (!player->loadTexture("assets/images/player/Slime1_Death.png", "player_death", renderer)) {
        SDL_Log("Failed to load player death texture");
        return false;
    }
    if (!player->loadTexture("assets/images/player/heart.png", "heart", renderer)) {
        SDL_Log("Failed to load heart texture");
        return false;
    }
    if (!player->loadTexture("assets/images/player/status.png", "status", renderer)) {
        SDL_Log("Failed to load status texture");
        return false;
    }
    //load Enemy
    for(int i = 1; i <= 2; i++) {
        string c = to_string(i);
        if (!Resources::Instance()->load("assets/images/enemies/1_Idle.png", "enemy1", renderer)) {
            SDL_Log("Failed to load enemy  %d Texture", i);
            return false;
        }
        if (!Resources::Instance()->load("assets/images/enemies/" + c +"_Attack.png", "enemy" + c + "_attack", renderer)) {
            SDL_Log("Failed to load enemy %d Texture", i);
            return false;
        }
        if (!Resources::Instance()->load("assets/images/enemies/" + c + "_Walk.png", "enemy" + c + "_walk", renderer)) {
            SDL_Log("Failed to load enemy %d Texture", i);
            return false;
        }
        if (!Resources::Instance()->load("assets/images/enemies/" + c + "_Death.png", "enemy" + c +"_death", renderer)) {
            SDL_Log("Failed to load enemy %d Texture", i);
            return false;
        }
        if (!Resources::Instance()->load("assets/images/enemies/" + c + "_Effect.png", "enemy" + c + "_effect", renderer)) {
            SDL_Log("Failed to load enemy %d Texture, i");
            return false;
        }
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
    //Load MainMenu
    if (!Resources::Instance()->load("assets/images/Menu/1.png", "MainMenu1", renderer)) {
        SDL_Log("Failed to load mainMenu");
        return false;
    }
    if (!Resources::Instance()->load("assets/images/Menu/2.png", "MainMenu2", renderer)) {
        SDL_Log("Failed to load mainMenu");
        return false;
    }
    //Load Pause
    if (!Resources::Instance()->load("assets/images/Pause.png", "Pause", renderer)) {
        SDL_Log("Failed to load pause");
        return false;
    }
    //Load Endless
    if (!Resources::Instance()->load("assets/images/ENDLESS.png", "EndLess", renderer)) {
        SDL_Log("Failed to load Endless");
        return false;
    }
    //Load GameOver
    if (!Resources::Instance()->load("assets/images/YOU DIE/1.png", "GameOver_Good",renderer)) {
        SDL_Log("Failed to load GameOver");
        return false;
    }
    if (!Resources::Instance()->load("assets/images/YOU DIE/2.png", "GameOver_Amazing",renderer)) {
        SDL_Log("Failed to load GameOver");
        return false;
    }
    if (!Resources::Instance()->load("assets/images/YOU DIE/3.png", "GameOver_TryBetter",renderer)) {
        SDL_Log("Failed to load GameOver");
        return false;
    }
    //Load items
    for (int i = 4; i <= 7; i++) {
        if (!Resources::Instance()->load("assets/images/items/" + to_string(i - 3) + ".png","Item" + to_string(i), renderer  )) {
            SDL_Log("Failed to load item");
            return false;
        }
    }
    //Load Tutorial
    for (int i = 1; i <= 3; i++) {
        if (!Resources::Instance()->load("assets/images/TUTORIAL/" + to_string(i) + ".png", "Tutorial" + to_string(i), renderer  )) {
            SDL_Log("Failed to load tutorial");
            return false;
        }
    }
    return true;
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
                    SDL_Rect GameOver = {275, 125, 350, 350};
                    int buttonWidth = 82;
                    int buttonHeight = 75;
                    SDL_Rect RestartRect = {
                        GameOver.x + 63,
                        GameOver.y + 269,
                        buttonWidth,
                        buttonHeight
                    };

                    SDL_Rect MainMenuRect = {
                        GameOver.x + 211,
                        GameOver.y + 269,
                        buttonWidth,
                        buttonHeight
                    };

                    if (mouseX >= RestartRect.x && mouseX <= RestartRect.x + buttonWidth &&
                        mouseY >= RestartRect.y && mouseY <= RestartRect.y + buttonHeight) {
                        // Start game
                        SDL_Log("click");
                        current_state = GAME_STATE::PLAYING;
                        resetGame();
                        } else if (mouseX >= MainMenuRect.x && mouseX <= MainMenuRect.x + buttonWidth &&
                            mouseY >= MainMenuRect.y && mouseY <= MainMenuRect.y + buttonHeight) {
                            current_state = GAME_STATE::MAIN_MENU;
                            resetGame();
                        }
                }
            break;
            default:
                break;
        }
    }
}

void Game::handleMainMenuEvents() {
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

                    int buttonWidth = 180;
                    int buttonHeight = 55;

                    SDL_Rect endlessbuttonRect = {
                        502,
                        465,
                        buttonWidth,
                        buttonHeight
                    };

                    SDL_Rect exitbuttonRect = {
                        23,32,
                        76, 85
                    };
                    SDL_Rect musicButtonRect = {
                        791, 401,
                        97, 100
                    };
                    SDL_Rect tutorialButtonRect = {
                        791,274,
                        97, 100
                    };
                    if (mouseX >= endlessbuttonRect.x && mouseX <= endlessbuttonRect.x + endlessbuttonRect.w &&
                        mouseY >= endlessbuttonRect.y && mouseY <= endlessbuttonRect.y + endlessbuttonRect.h) {
                        current_state = GAME_STATE::PLAYING;
                    } else if (mouseX >= exitbuttonRect.x && mouseX <= exitbuttonRect.x + exitbuttonRect.w &&
                        mouseY >= exitbuttonRect.y && mouseY <= exitbuttonRect.y + exitbuttonRect.h) {
                        isRunning = false;
                    } else if (mouseX >= musicButtonRect.x && mouseX <= musicButtonRect.x + musicButtonRect.w &&
                        mouseY >= musicButtonRect.y && mouseY <= musicButtonRect.y + musicButtonRect.h) {
                        SDL_Log("tes");
                        Menu_id = 1 - Menu_id;
                    } else if (mouseX >= tutorialButtonRect.x && mouseX <= tutorialButtonRect.x + tutorialButtonRect.w &&
                        mouseY >= tutorialButtonRect.y && mouseY <= tutorialButtonRect.y + tutorialButtonRect.h) {
                        tutorial = true;
                    }
                }
                break;
            default:
                break;
        }
    }
}
void Game::handlePauseMenuEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    current_state = GAME_STATE::PLAYING;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    int buttonWidth = 82;
                    int buttonHeight = 75;

                    SDL_Rect ResetRect = {
                        275 + 22, 250 + 12,
                        82, 75
                    };
                    SDL_Rect ContinueRect = {
                            275 + 130, 250 + 12,
                        82, 75
                    };
                    SDL_Rect MainMenuRect = {
                        275 + 247, 250 + 12,
                        82, 75
                    };
                    if (mouseX >= ResetRect.x && mouseX <= ResetRect.x + ResetRect.w &&
                        mouseY >= ResetRect.y && mouseY <= ResetRect.y + ResetRect.h) {
                        resetGame();
                        current_state = GAME_STATE::PLAYING;
                    } else if (mouseX >= ContinueRect.x && mouseX <= ContinueRect.x + ContinueRect.w &&
                        mouseY >= ContinueRect.y && mouseY <= ContinueRect.y + ContinueRect.h) {
                        current_state = GAME_STATE::PLAYING;
                    } else if (mouseX >= MainMenuRect.x && mouseX <= MainMenuRect.x + MainMenuRect.w &&
                        mouseY >= MainMenuRect.y && mouseY <= MainMenuRect.y + MainMenuRect.h) {
                        resetGame();
                        current_state = GAME_STATE::MAIN_MENU;
                    }
                }
                break;
            default:
                break;
        }
    }
}

void Game::handleGameTutorial() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (time_tutorial_popup < 0) tutorial = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (time_tutorial_popup < 0) tutorial = false;
                break;
            default:
                break;
        }
    }
}

void Game::renderMainMenu() {
    Resources::Instance()->render("MainMenu" + to_string(Menu_id + 1), 0, 0, 900, 600, 900, 600, renderer, flip);
}

void Game::renderPause() {
    SDL_Rect PauseRect = {275 ,250 , 350, 100};
    Resources::Instance()->render("Pause", PauseRect.x, PauseRect.y, PauseRect.w, PauseRect.h, PauseRect.w,PauseRect.h, renderer, flip);
}

void Game::renderTutorial(const int id) {
    time_tutorial_popup--;
    Resources::Instance()->render("Tutorial" + to_string(id), 150, 125, 600, 350, 600, 350, renderer, flip);
}

void Game::renderGameOver() {
    SDL_Rect GameOver = {275, 125, 350, 350};
    if (score < 10) {
        Resources::Instance()->render("GameOver_TryBetter", GameOver.x, GameOver.y, GameOver.w, GameOver.h, GameOver.w, GameOver.h, renderer, flip);

    } else if (score < 50) {
        Resources::Instance()->render("GameOver_Good", GameOver.x, GameOver.y, GameOver.w, GameOver.h, GameOver.w, GameOver.h, renderer, flip);

    } else {
        Resources::Instance()->render("GameOver_Amazing", GameOver.x, GameOver.y, GameOver.w, GameOver.h, GameOver.w, GameOver.h, renderer, flip);

    }

    std::string scoreText = std::to_string(score);

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

    int textWidth = scoreSurface->w * 2;
    int textHeight = scoreSurface->h * 2;

    SDL_Rect renderScore = {
        275 + 200,
        125 + 145,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, scoreTexture, nullptr, &renderScore);

    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
}



void Game::renderScore() {
    int ScoreWidth = 140;
    int ScoreHeight = 260;

    Resources::Instance()->render("EndLess",
        SCREEN_WIDTH - ScoreWidth - 24, 24, ScoreWidth, ScoreHeight,
        ScoreWidth, ScoreHeight, renderer, flip);

    std::string scoreText = std::to_string(score);
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
    int textWidth = scoreSurface->w * 4;
    int textHeight = scoreSurface->h * 4;
    SDL_Rect renderScore = {
        (SCREEN_WIDTH - ScoreWidth - 24) + (ScoreWidth / 2) - (textWidth/2),
        135,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, scoreTexture, nullptr, &renderScore);

    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
}

// Game run loop
void Game::run() {
    SDL_Event event;

    while (isRunning) {
        update();
        render();
        SDL_Delay(15);
    }
}

void Game::resetGame() {
    player->set_direct(0);
    player->setXY(6,6);
    player->heal("full");
    bombs.clear();
    enemies.clear();
    explosions.clear();
    map.load("assets/maps/level1.txt");
    score = 0;
    time_EnemySpawn = 0;
    tutorial = 0;
}



void Game::update() {
    switch (current_state) {
        case GAME_STATE::PLAYING:
            if (tutorial == true) {
                handleGameTutorial();
                break;
            }
            this->inputHandler.handleEvents(this->isRunning, *this->player, *this, current_state, map, bombs);
            if (time_EnemySpawn%(TIME_1SECOND/2) == 0) {
                if (cnt_tutorial == 0) {
                    cnt_tutorial = 1;
                    tutorial = true;
                }
                enemies.emplace_back(1, 1,1,renderer);
                enemies.emplace_back(1, 15,15,renderer);
                if (score >= 10) {
                    if (cnt_tutorial == 1) {
                        cnt_tutorial = 2;
                        tutorial = true;
                    }
                    enemies.emplace_back(2, 15,1,renderer);
                    enemies.emplace_back(2, 1,15,renderer);
                    enemies.emplace_back(1, 1,6,renderer);
                    enemies.emplace_back(1, 15,6,renderer);
                }
                if (score > 50) {
                    enemies.emplace_back(2, 6,1,renderer);
                    enemies.emplace_back(2, 6, 15,renderer);
                }
            }
            time_EnemySpawn++;
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
        // update enemies
        for (auto it = enemies.begin(); it != enemies.end();) {
            it->update(static_cast<int> (logic.round_2(player->getX())),static_cast<int> (logic.round_2(player->getY())), map); //Use it pointer access
            if (it->isDeath() == 3) {
                score++;
                it = enemies.erase(it);
            }
            else ++it;
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
        break;
        case GAME_STATE::MAIN_MENU:
            if (tutorial == true) handleGameTutorial();
            else handleMainMenuEvents();
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
            if (current_explosion_X == it->getX() && current_explosion_Y == it->getY())
            {it -> Death();}
            ++it;
        }
        if (current_explosion_Y == player->getY() && current_explosion_X == player->getX() && explosion.get_hurt() == false) {
            player->hurt();
            explosion.is_hurt();
        }
        explosion.render(renderer,explosionTexture[0], 0,0);
        for (auto u : position) {
            for (int i = 1; i < player->size(); i++) {
                current_explosion_X = explosion.get_X() + u.first * i;
                current_explosion_Y = explosion.get_Y() + u.second * i;
                if (map.limit(current_explosion_X, current_explosion_Y) == '8' || ( map.limit(current_explosion_X, current_explosion_Y) == '2' && explosion.isFinished() == true)) {
                    map.Create_map('0', current_explosion_X, current_explosion_Y);
                    break;
                } else if (map.limit(current_explosion_X, current_explosion_Y) > '1') {
                    break;
                }
                if (current_explosion_Y == player->getY() && current_explosion_X == player->getX() && explosion.get_hurt() == false) {
                    player->hurt();
                    explosion.is_hurt();
                }
                explosion.render(renderer,explosionTexture[i], u.first*i,u.second*i);
                for (auto it = enemies.begin(); it != enemies.end();) {
                    if (current_explosion_X == it->getX() && current_explosion_Y == it->getY())
                    {it -> Death();}
                    ++it;
                }
            }

        }
    }

    for (auto& enemy : enemies) {
        enemy.render(renderer, player->getX(),player->getY());
        if (enemy.is_kill() && !enemy.is_hurt()) {
            player->hurt();
            enemy.hurt_player();
        }
    }
    player->render_player(renderer);

    if (player->get_health() == 0) {
        if (player->isDeath() == 0) player->Death(1);
        current_state = GAME_STATE::GAME_OVER;
    }
    switch (current_state) {
        case GAME_STATE::MAIN_MENU:
            renderMainMenu();
            if (tutorial == true) renderTutorial(1);
            break;
        case GAME_STATE::GAME_OVER:
            if (player->isDeath() == 2) renderGameOver();
            break;
        case GAME_STATE::PLAYING:
            if (tutorial == true) {
                if (score < 10) renderTutorial(2);
                else renderTutorial(3);
            }
        break;
            break;
        case GAME_STATE::PAUSE:
            renderPause();
            break;
        default:
            break;

    }
    SDL_RenderPresent(renderer);
}




void Game::cleanup() {
    if (this->player) {
        delete player;
        player = nullptr;
    }
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