
#include "player.h"
#include "map.h"
#include "render/resources.h"

#include <chrono>
#include <SDL.h>  // Required for SDL_Log
#include <bits/atomic_wait.h>

#include "constant.h"

Player::Player(int startX, int startY) : x(startX), y(startY), speed(SPEED_PLAYER),
bombLimit(2), direct(0), timer(0), dx(0), dy(0), flip(SDL_FLIP_NONE), last_x(startX), last_y(startY) {
    Font = TTF_OpenFont("assets/font/Karma Suture.otf", 24);
}
bool Player::loadTexture(const string &filePath, const string &id, SDL_Renderer* renderer) {
    if (Resources::Instance()->load(filePath, id, renderer)) {
        textureID = id;
        return true;
    }
    return false;
}

void Player::render_player(SDL_Renderer* renderer) {
    time_immortal--;
    timer++;
    SDL_Rect playerRect = { CENTER_X +x * TILE_SIZE - 16,CENTER_Y + y * TILE_SIZE - 16, SIZE_TEXTURE_PLAYER, SIZE_TEXTURE_PLAYER};
    SDL_Rect player_frame;
    int frame;
    STATUS player = {"player", PRESS_DELAY, NUM_FRAME_IDLE[0]};
    if (!walk) {
        if (death == 1) player = {"player_death", ENEMIES_DEATH_SPEED, NUM_FRAME_DEATH};
            else player = {"player", PRESS_DELAY, NUM_FRAME_IDLE[0]};
        frame = timer/(player.speed_frame/player.num_frame);
        if (frame >= player.num_frame) {
            if (death == 1) {
                death = 2;
                frame = 11;
            } else {
                timer = 0;
                frame = 0;
            }
        }
    } else {
        player = {"player_walk", PLAYER_WALK_FRAME, NUM_FRAME_WALK[0]};
        frame = timer/(player.speed_frame/player.num_frame);
        float move_frame = frame;
        if (frame >= player.num_frame) {
            frame = 0;
        }
        playerRect.x = CENTER_X + static_cast<int>((last_x + move_frame * (x - last_x)/(player.num_frame - 1)) * TILE_SIZE);
        playerRect.y = CENTER_Y + static_cast<int>((last_y + move_frame * (y - last_y)/(player.num_frame - 1)) * TILE_SIZE);
        if (abs(playerRect.x - CENTER_X - last_x * TILE_SIZE) > TILE_SIZE) playerRect.x = CENTER_X + x * TILE_SIZE - 16;
        else playerRect.x -= 16;
        if (abs(playerRect.y - CENTER_Y - last_y * TILE_SIZE) > TILE_SIZE) playerRect.y = CENTER_Y + y * TILE_SIZE - 16;
        else playerRect.y -= 16;
        if (timer >= player.speed_frame) {
            timer = 0;
            walk = false;
        }
    }
    textureID = player.IMG;
    Resources::Instance()->renderFrame(
        textureID,
        playerRect.x, playerRect.y,
        playerRect.w, playerRect.h,
        direct, frame,
        64, 64,
        renderer,
        flip
        );

    // STATUS BOARD
    SDL_Rect rect = {24, 24, 140, 260};

    Resources::Instance()->render(
        "status",
        rect.x, rect.y,
        rect.w, rect.h,
        rect.w, rect.h,
        renderer, flip
        );

    Resources::Instance()->renderFrame(
        textureID,
        0, 0,
        128, 128,
        direct, frame,
        64, 64,
        renderer,
        flip
        );
    SDL_Rect status = {114 , 48, 32, 32};
    Resources::Instance()->renderFrame("heart",
        status.x, status.y, status.w, status.h,
        1, 2 - health,
        158, 158,
        renderer, flip);

    //BOMB_LIMIT
    write_status(renderer, bombLimit, 0);
    //SHIELD
    write_status(renderer, shield, 1);
    //LENGTH
    write_status(renderer, size_explode - 1, 2);
};

void Player::write_status(SDL_Renderer* renderer,const int &number,const int &size) {
    int StatusWidth = 140;
    int StatusHeight = 260;
    std::string Text = std::to_string(number);
    SDL_Surface* Surface = TTF_RenderText_Solid(Font, Text.c_str(), Color);
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
    int textWidth = Surface->w;
    int textHeight = Surface->h;
    SDL_Rect renderStatus = {
        24 + StatusWidth/2 + StatusWidth/4 - (textWidth/2) + 2,
        24 + StatusHeight/4 + size * 60 + (StatusHeight/4 - textHeight)/2,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, statusTexture, nullptr, &renderStatus);

    SDL_FreeSurface(Surface);
    SDL_DestroyTexture(statusTexture);
}

void Player::update(const int &direction, const int &next_x, const int &next_y, Map& map) {
    if (!walk) {
        direct =  direction;
        dx = next_x;
        dy = next_y;
        last_x = x;
        last_y = y;
        if (!(map.limit(x + dx * speed, y + dy * speed) == '2') && !(map.limit(x + dx * speed, y + dy* speed) == '3')) {
            x += dx;
            y += dy;
            walk = true;
            timer = 0;
            if (map.limit(x, y) >= '4' && map.limit(x, y) <= '8') {
                switch (map.limit(x,y)) {
                    case '4':
                        if (size_explode <= 5) size_explode++;
                    break;
                    case '5':
                        heal("");
                        break;
                    case '6':
                        shield++;
                        break;
                    case '7':
                        if (bombLimit < 5) bombLimit++;
                        break;
                    case '8':
                        if (health > 0) health--;

                        break;
                    default: break;
                }
                map.Create_map('0', x, y);
            }
        } else {
            SDL_Log("cant walk");
        }
    }
}

int Player::getBombLimit() const {
    return bombLimit;
}

int Player::getX() const {
    return x;
}

int Player::getY() const{
    return y;
}

int Player::get_last_x() const {
    return last_x;
}

int Player::get_last_y() const {
    return last_y;
}

