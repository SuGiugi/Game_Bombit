#include "game.h"
#include "../scripts/Player/player.h"
#include "../scripts/Object/bomb.h"
#include "../scripts/Map/map.h"
#include "../scripts/Render/resources.h"
#include "../scripts/Info/constant.h"
#include "../scripts/Object/explosion.h"
#include "../scripts/Enemy/enemy.h"
#include <cmath>
#include <algorithm>
#include <cstring>
#include <SDL_image.h>

// Khoi tao thong so
Game::Game() : window(nullptr), renderer(nullptr), map("assets/maps/level1.txt"), player(nullptr),
               Font(nullptr), GameOverTexture(nullptr), Music(nullptr), gHigh(nullptr){}

bool Game::init() { // Khoi tao cua so, hinh anh, am thanh, phong chu
    if (SDL_Init(SDL_INIT_VIDEO || SDL_INIT_AUDIO) < 0) {
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
    // Game bat dau chay
    isRunning = true;


    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 8, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    //Load Font
    Font = TTF_OpenFont("assets/font/VCR_OSD_MONO_1.001.ttf", 24);
    if (!Font) {
        SDL_Log("Failed to load font! SDL_ttf Error: %s", TTF_GetError());
         return false;
    }

    //Khoi tao hinh anh (img)
    if (!Resources::Instance()->init()) {
        SDL_Log("Resources could not be initialized");
        return false;
    } else {
        SDL_Log("Resources initialized Successful");
    }

    // Tao player tai vi tri chinh giua ban do 8 8
    player = new Player(8, 8);

    // Tao hinh anh
    if (!loadAssets()) {
        return false;
    }

    // Tao am thanh
    if (!loadMedia()) {
        return false;
    }

    // Dieu chinh am nhac va bat dau chou nhac
    Mix_VolumeChunk(gHigh, 128);
    Mix_VolumeChunk(gPlace, 64);
    Mix_VolumeChunk(gDeath, 64);
    Mix_VolumeChunk(gExplosion, 64);
    Mix_VolumeMusic(20);
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(Music, -1);
    }
    return true;
}

bool Game::loadMedia() {
    Music = Mix_LoadMUS("assets/sounds/The Fairy Dance.mp3");
    if (Music == nullptr) {
        SDL_Log("Failed to load music");
        return false;
    }
    gDeath = Mix_LoadWAV("assets/sounds/Death.wav");

    gHigh = Mix_LoadWAV("assets/sounds/Press.wav");
    gExplosion = Mix_LoadWAV("assets/sounds/Explosion.wav");
    gPlace = Mix_LoadWAV("assets/sounds/Place.wav");
    if (gPlace == nullptr || gHigh == nullptr || gExplosion == nullptr || gDeath == nullptr) {
        SDL_Log("Failed to load wave");
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
    for(int i = 1; i <= NUMBER_ENEMIES; i++) {
        string c = to_string(i);
        if (!Resources::Instance()->load("assets/images/enemies/" + c +"_Idle.png", "enemy" + c, renderer)) {
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
    for (int i = 1; i <= 3; i++) {
        if (!Resources::Instance()->load("assets/images/background/" + to_string(i) +".png",
            "background" + to_string(i), renderer)) {
            SDL_Log("Failed to load background texture.");
            return false;
        }
    }
    //load bomb
    if (!Resources::Instance()->load("assets/images/bomb/1.png", "bomb1", renderer)) {
        SDL_Log("Failed to load bomb");
        return false;
    }
    if (!Resources::Instance()->load("assets/images/bomb/2.png", "bomb2", renderer)) {
        SDL_Log("Failed to load bomb");
        return false;
    }
    for (int i = 1; i <= 3; i++) {
        if (!Resources::Instance()->load("assets/images/ground/" + to_string(i) + ".png", "grass" + to_string(i), renderer)) {
            SDL_Log("Failed to load map texture.");
            return false;
        }
    }
    if (!Resources::Instance()->load("assets/images/tree.png", "tree", renderer)) {
        SDL_Log("Failed to load object texture.");
        return false;
    }
    if (!Resources::Instance()->load("assets/images/shrub.png", "shrub", renderer)) {
        SDL_Log("Failed to load shrub texture.");
        return false;
    }
    if (!Resources::Instance()->load("assets/images/rock.png", "rock", renderer)) {
        SDL_Log("Failed to load rock texture.");
        return false;
    }
    //Load explosion;
    for (int i = 0; i < 6; i++) {
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
    for (int i = 1; i <= 4; i++) {
        if (!Resources::Instance()->load("assets/images/TUTORIAL/" + to_string(i) + ".png", "Tutorial" + to_string(i), renderer  )) {
            SDL_Log("Failed to load tutorial");
            return false;
        }
    }
    return true;
}

// su kien khi GameOver
void Game::handleGameOverEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
            break;
            case SDL_MOUSEBUTTONDOWN:
                Mix_PlayChannel(-1, gHigh, 0);
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    SDL_Rect GameOver = {275, 125, 350, 350};
                    int buttonWidth = 82;
                    int buttonHeight = 75;
                    // Nut choi lai
                    SDL_Rect RestartRect = {
                        GameOver.x + 63,
                        GameOver.y + 269,
                        buttonWidth,
                        buttonHeight
                    };
                    // Nut ve MainMenu
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
// su kien khi dang o GameState::MainMenu
void Game::handleMainMenuEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
            break;
            case SDL_MOUSEBUTTONDOWN:
                Mix_PlayChannel(-1, gHigh, 0);
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    int buttonWidth = 180;
                    int buttonHeight = 55;
                    // Nut Start
                    SDL_Rect EndlessbuttonRect = {
                        502,
                        465,
                        buttonWidth,
                        buttonHeight
                    };
                    // Nut Exit
                    SDL_Rect ExitbuttonRect = {
                        23,32,
                        76, 85
                    };
                    // Nut Am thanh
                    SDL_Rect musicButtonRect = {
                        791, 401,
                        97, 100
                    };
                    // Nut huong dan
                    SDL_Rect tutorialButtonRect = {
                        791,274,
                        97, 100
                    };
                    if (mouseX >= EndlessbuttonRect.x && mouseX <= EndlessbuttonRect.x + EndlessbuttonRect.w &&
                        mouseY >= EndlessbuttonRect.y && mouseY <= EndlessbuttonRect.y + EndlessbuttonRect.h) {
                        // bat dau choi va chon 1 map level bat ky
                        player->Start_game();
                        int random = rand()%NUMBER_LEVEL + 1;
                        map.load("assets/maps/level"+ to_string(random) + ".txt");
                        current_state = GAME_STATE::PLAYING;
                    } else if (mouseX >= ExitbuttonRect.x && mouseX <= ExitbuttonRect.x + ExitbuttonRect.w &&
                        mouseY >= ExitbuttonRect.y && mouseY <= ExitbuttonRect.y + ExitbuttonRect.h) {
                        // Thoat khoi game
                        isRunning = false;
                    } else if (mouseX >= musicButtonRect.x && mouseX <= musicButtonRect.x + musicButtonRect.w &&
                        mouseY >= musicButtonRect.y && mouseY <= musicButtonRect.y + musicButtonRect.h) {
                        // Tat am thanh nen
                        if (Mix_PausedMusic() == 1) {
                            Mix_ResumeMusic();
                        } else {
                            Mix_PauseMusic();
                        }
                        Menu_id = 1 - Menu_id;
                    } else if (mouseX >= tutorialButtonRect.x && mouseX <= tutorialButtonRect.x + tutorialButtonRect.w &&
                        mouseY >= tutorialButtonRect.y && mouseY <= tutorialButtonRect.y + tutorialButtonRect.h) {
                        // Bat Huong dan
                        tutorial = true;
                    } else {
                        // Nut tang giam chi so
                        SDL_Rect Button {
                            130, 318,
                            59, 59
                        };
                        for (int i = 0; i < 2; i++) {
                            for (int j = 0; j < 3; j++) {
                                if (mouseX >= Button.x + 166 * i && mouseX <= Button.x + Button.w + 166 * i &&
                                    mouseY >= Button.y + 74 * j && mouseY <= Button.y + Button.h + 74 * j) {
                                    // Tuy vao vi tri bam tang giam so khien, so bomb, kich thuoc bomb
                                        player->setup(i, j);
                                }
                            }
                        }
                        // lua map (background)
                        Button = {
                            430, 343,
                            40, 80
                        };
                        for (int i = 0; i <= 1; i++) {
                            if (mouseX >= Button.x + 288 * i && mouseX <= Button.x + Button.w + 288 * i &&
                                mouseY >= Button.y && mouseY <= Button.y + Button.w) {
                                if (i == 0) {
                                    background_Texture--;
                                } else {
                                    background_Texture++;
                                }
                                background_Texture = (background_Texture + 3)%3;
                                if (background_Texture == 0) {background_Texture = 3;}
                            }
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
}
// su kien khi pauseGame
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
                Mix_PlayChannel(-1, gHigh, 0);
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    // Nut choi lai
                    SDL_Rect ResetRect = {
                        275 + 22, 250 + 12,
                        82, 75
                    };
                    //Nut tiep tuc
                    SDL_Rect ContinueRect = {
                            275 + 130, 250 + 12,
                        82, 75
                    };
                    //Nut ve MainMenu
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
// Su kien khi dang o huong dan
void Game::handleGameTutorial() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    player->set_time_imortal();
                    tutorial = false;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                Mix_PlayChannel(-1, gHigh, 0);
                if (event.button.button == SDL_BUTTON_LEFT) {
                    //Nut tat huong dan
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    SDL_Rect Button_Exit{
                        703, 133,
                        53 , 54
                    };
                    if (mouseX >= Button_Exit.x && mouseX <= Button_Exit.x + Button_Exit.w &&
                        mouseY >= Button_Exit.y && mouseY <= Button_Exit.y + Button_Exit.h) {
                        player->set_time_imortal();
                        tutorial = false;
                    }
                }
                break;
            default:
                break;
        }
    }
}
// tai Menu
void Game::renderMainMenu() {
    // tai man hinh chinh
    Resources::Instance()->render("MainMenu" + to_string(Menu_id + 1), 0, 0, 900,
        600, 900, 600, renderer, flip);
    int shield, bomb, length;

    // lay thong so nguoi choi truoc khi bat dau
    player->get_status(shield,  length, bomb);
    write_status(renderer, bomb, 0);
    write_status(renderer, shield, 1);
    write_status(renderer, length - 1, 2);
    Resources::Instance()->render("background" + to_string(background_Texture),492, 314,
        204, 136, 2304, 1296, renderer, flip );
}

// viet thong so nguoi choi ra man hinh
void Game::write_status(SDL_Renderer* renderer,const int &number,const int &size) const {
    int StatusWidth = 107;
    string Text = to_string(number);
    SDL_Surface* Surface = TTF_RenderText_Solid(Font, Text.c_str(), textColor);
    if (!Surface) {
        SDL_Log("Unable to render score text surface! SDL_ttf Error: %s", TTF_GetError());
        return;
    }
    SDL_Texture* statusTexture = SDL_CreateTextureFromSurface(renderer, Surface);
    if (!statusTexture) {
        SDL_Log("Unable to create score texture! SDL Error: %s", SDL_GetError());
        SDL_FreeSurface(Surface);
        return;
    }
    int textWidth = static_cast<int>(Surface->w * 2.5);
    int textHeight = static_cast<int>(Surface->h * 2.5);
    SDL_Rect renderStatus = {
        189 + (107 - textWidth)/2,
        310 + (textHeight + 20) * size,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, statusTexture, nullptr, &renderStatus);
    SDL_FreeSurface(Surface);
    SDL_DestroyTexture(statusTexture);
}

// tai hinh anh khi Pause Game
void Game::renderPause() {
    SDL_Rect PauseRect = {275 ,250 , 350, 100};
    Resources::Instance()->render("Pause", PauseRect.x, PauseRect.y, PauseRect.w, PauseRect.h, PauseRect.w,PauseRect.h, renderer, flip);
}

// tai hinh anh huong dan
void Game::renderTutorial(const int &id) {
    Resources::Instance()->render("Tutorial" + to_string(id), 150, 125, 600, 350, 600, 350, renderer, flip);
}

// tai hinh anh khi game over
void Game::renderGameOver() {
    // tuy vao diem so game over se khac nhau
    SDL_Rect GameOver = {275, 125, 350, 350};
    if (score < 10) {
        Resources::Instance()->render("GameOver_TryBetter", GameOver.x, GameOver.y, GameOver.w, GameOver.h, GameOver.w, GameOver.h, renderer, flip);

    } else if (score < 50) {
        Resources::Instance()->render("GameOver_Good", GameOver.x, GameOver.y, GameOver.w, GameOver.h, GameOver.w, GameOver.h, renderer, flip);

    } else {
        Resources::Instance()->render("GameOver_Amazing", GameOver.x, GameOver.y, GameOver.w, GameOver.h, GameOver.w, GameOver.h, renderer, flip);

    }

    // tai diem cua nguoi choi ra giua man hinh
    std::string scoreText = std::to_string(score);

    SDL_Surface* scoreSurface = TTF_RenderText_Solid(Font, scoreText.c_str(), Color);
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


// tai diem cua nguoi choi khi dang choi
void Game::renderScore() {
    int ScoreWidth = 140;
    int ScoreHeight = 260;

    Resources::Instance()->render("EndLess",
        SCREEN_WIDTH - ScoreWidth - 24, 24, ScoreWidth, ScoreHeight,
        ScoreWidth, ScoreHeight, renderer, flip);

    std::string scoreText = std::to_string(score);
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(Font, scoreText.c_str(), Color);
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

//tai lai game
void Game::resetGame() {
    player->set_direct(0);
    player->setXY(8,8);
    player->heal("full");
    bombs.clear();
    enemies.clear();
    explosions.clear();
    int random = rand()%NUMBER_LEVEL + 1;
    map.load("assets/maps/level"+ to_string(random) + ".txt");
    score = 0;
    time_EnemySpawn = 0;
    cnt_tutorial = 0;
}

// UPDATE
void Game::update() {
    switch (current_state) {
        case GAME_STATE::PLAYING:
            if (tutorial == true) {
                handleGameTutorial();
                break;
            }
            this->inputHandler.handleEvents(this->isRunning, *this->player, *this, current_state, map, bombs);
             // tao quai tuy theo diem so cua nguoi choi hien tai
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
                }
                if (score >= 20) {
                    if (cnt_tutorial == 2) {
                        cnt_tutorial = 3;
                        tutorial = true;
                    }
                    enemies.emplace_back(3, 6,1,renderer);
                    enemies.emplace_back(3, 6, 15,renderer);
                }
                if (score >= 40) {
                    srand(time(NULL));
                    int random = (rand())%3 + 1;
                    enemies.emplace_back(random, 1,6,renderer);
                    enemies.emplace_back(random, 15,6,renderer);
                }
                if (score >= 50) {
                    int random = (rand() + 1)%3 + 1;
                    enemies.emplace_back(random, 9,9,renderer);
                }
            }
            time_EnemySpawn++;
            // Update Bombs
            for (auto it = bombs.begin(); it != bombs.end();) {
                it->update();
                if (it->isExploded()) {
                    // Kiem tra bomb no hay chua
                    map.Create_map('0', it->getX(), it->getY());
                    Mix_PlayChannel(-1, gExplosion, 0);
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
                // Kiem tra xem dich chet hay chua + tao vat pham ngau nhien khi chet
                char a = map.generate_item();
                if (a != '0') map.Create_map(a, it->getX(), it->getY());
                score++;
                it = enemies.erase(it);
            }
            else ++it;
        }
        //Update Explosions
        for (auto it = explosions.begin(); it != explosions.end();) {
            if (it->isFinished()) {
                // Kiem tra xem vu no da xong hay chua
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
    // Tai background tro choi
    Resources::Instance()->render("background" + to_string(background_Texture),0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 2304, 1296, renderer, flip );
    // Tai diem nguoi choi
    renderScore();
    //Render the Map:
    map.render(renderer, background_Texture);

    //Draw Bombs:
    for (const auto& bomb : bombs) {
        SDL_Rect bombRect = {CENTER_X + bomb.getX() * TILE_SIZE + 3, CENTER_Y + bomb.getY() * TILE_SIZE + 3, TILE_SIZE - 6, TILE_SIZE - 6};
        // Khi bomb gan no thi nhap nhay
        if (bomb.gettime() <= TIME_EXPLODE/4  && bomb.gettime()%10 == 0 ) {
            Resources::Instance()->render("bomb2",
            bombRect.x, bombRect.y,
            bombRect.w, bombRect.h,
            500, 500, renderer, flip);
        } else {
            Resources::Instance()->render("bomb1",
                bombRect.x, bombRect.y,
                bombRect.w, bombRect.h,
                500, 500, renderer, flip);
        }
    }
    // update vu no
    for (auto& explosion : explosions) {
        explosion.update();
        // kiem tra xem vu no co cham bat ki thu gi hay khong
        int current_explosion_X = explosion.get_X();
        int current_explosion_Y = explosion.get_Y();
        for (auto it = enemies.begin(); it != enemies.end();) {
            if (current_explosion_X == it->getX() && current_explosion_Y == it->getY()) {
                if (it -> isDeath() == 0) Mix_PlayChannel(-1, gDeath, 0);
                it -> Death();
            }
            ++it;
        }
        if (current_explosion_Y == player->getY() && current_explosion_X == player->getX() && explosion.get_hurt() == false) {
            player->hurt("");
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
                    player->hurt("");
                    explosion.is_hurt();
                }
                explosion.render(renderer,explosionTexture[i], u.first*i,u.second*i);
                for (auto it = enemies.begin(); it != enemies.end();) {
                    if (current_explosion_X == it->getX() && current_explosion_Y == it->getY()) {
                        if (it -> isDeath() == 0) Mix_PlayChannel(-1, gDeath, 0);
                        it -> Death();
                    }
                    ++it;
                }
            }

        }
    }
    // render ke dich va kiem tra xem co danh vao nguoi choi hay khong
    for (auto& enemy : enemies) {
        enemy.render(renderer, player->getX(),player->getY());
        if (enemy.is_kill() && !enemy.is_hurt()) {
            SDL_Log("%d", enemy.is_hurt());
            if (enemy.ID() == 3) player->hurt("piercing");
            else player->hurt("");
            enemy.hurt_player();
        }
    }
    // kiem tra xem nguoi choi da chet hay chua
    if (player->get_health() == 0) {
        if (player->isDeath() == 0) {
            Mix_PlayChannel(-1, gDeath, 0);
            player->Death(1);
        }
        current_state = GAME_STATE::GAME_OVER;
    }

    // Draw Player:
    player->render_player(renderer);
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
                else if (score < 20) renderTutorial(3);
                else renderTutorial(4);
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
    player->close();
    Mix_FreeChunk(gDeath);
    Mix_FreeChunk(gExplosion);
    Mix_FreeChunk(gPlace);
    Mix_FreeChunk(gHigh);
    gHigh = NULL;
    gDeath = NULL;
    gPlace = NULL;
    gExplosion = NULL;
    Mix_FreeMusic(Music);
    Music = NULL;

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
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}