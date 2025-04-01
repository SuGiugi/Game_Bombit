
#include "player.h"
#include "map.h"
#include "render/resources.h"

#include <chrono>
#include <SDL.h>  // Required for SDL_Log
#include <bits/atomic_wait.h>

#include "constant.h"

Player::Player(int startX, int startY) : x(startX), y(startY), speed(SPEED_PLAYER),
bombLimit(2), direct(0), timer(0), dx(0), dy(0), flip(SDL_FLIP_NONE), last_x(startX), last_y(startY) {}
bool Player::loadTexture(const string &filePath, const string &id, SDL_Renderer* renderer) {
    if (Resources::Instance()->load(filePath, id, renderer)) {
        textureID = id;
        return true;
    }
    return false;
}

void Player::render_player(SDL_Renderer* renderer) {
    timer++;
    SDL_Rect playerRect = { CENTER_X + static_cast<int>(x * TILE_SIZE) - 16,CENTER_Y + static_cast<int>(y * TILE_SIZE) - 16, SIZE_TEXTURE_PLAYER, SIZE_TEXTURE_PLAYER};
    SDL_Rect player_frame;
    int frame;
    STATUS player = {"player", PRESS_DELAY, NUM_FRAME_IDLE, 0, 1};
    if (!walk) {
        player = {"player", PRESS_DELAY, NUM_FRAME_IDLE, 0, 1};
        frame = timer/(player.speed_frame/player.num_frame);
        if (frame >= player.num_frame) {
            timer = 0;
            frame = 0;
        }
    } else {
        player = {"player_walk", PLAYER_WALK_FRAME, NUM_FRAME_WALK, 0, 1};
        frame = timer/(player.speed_frame/player.num_frame);
        float move_frame = frame;
        if (frame >= player.num_frame) {
            frame = 0;
        }
        playerRect.x = CENTER_X + static_cast<int>((last_x + move_frame * (x - last_x)/(NUM_FRAME_WALK - 1)) * TILE_SIZE);
        playerRect.y = CENTER_Y + static_cast<int>((last_y + move_frame * (y - last_y)/(NUM_FRAME_WALK - 1)) * TILE_SIZE);
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
    SDL_Rect rect = {24, 24, 140, 80};

    SDL_SetRenderDrawColor(renderer, 212, 201, 190, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 3, 3, 3, 255);
    SDL_RenderDrawRect(renderer, &rect);

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
    SDL_Rect heart_frame = { 316 - 158 * health, 160, 158,158};
    Resources::Instance()->renderFrame("heart",
        114, 48, 32, 32,
        1, 2 - health,
        158, 158,
        renderer, flip);
};

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
            if (map.limit(x, y) == '4') {
                if (size_explode <= 5) size_explode++;
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

