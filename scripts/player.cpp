
#include "player.h"

#include <chrono>
#include <SDL.h>  // Required for SDL_Log
#include <bits/atomic_wait.h>

#include "constant.h"

Player::Player(float startX, float startY) : x(startX), y(startY), speed(SPEED_PLAYER), bombLimit(2), direct(0), timer(0) {}

void Player::move(float dx, float dy) {
    float movementAmountX = speed * dx;
    float movementAmountY = speed * dy;
    x += movementAmountX;
    y += movementAmountY;
    SDL_Log("Player moved to (%d, %d)", x, y); // Use SDL_Log
}

void Player::render_player(SDL_Renderer* renderer, SDL_Texture* player_texture, SDL_Texture* player_walk_texture, bool &walk) {
    timer++;
    SDL_Rect playerRect = { CENTER_X + static_cast<int>(x * TILE_SIZE) - 16,CENTER_Y + static_cast<int>(y * TILE_SIZE) - 16, SIZE_TEXTURE_PLAYER, SIZE_TEXTURE_PLAYER};
    SDL_Rect player_frame;
    int frame;
    STATUS player = {player_texture, PRESS_DELAY, NUM_FRAME_IDLE, 0, 1};
    if (!walk) {
        player = {player_texture, PRESS_DELAY, NUM_FRAME_IDLE, 0, 1};
        frame = timer/(player.speed_frame/player.num_frame);
        if (frame >= player.num_frame) {
            timer = 0;
            frame = 0;
        }
    } else {
        player = {player_walk_texture, PLAYER_WALK_FRAME, NUM_FRAME_WALK, 0, 1};
        frame = timer/(player.speed_frame/player.num_frame);
        float move_frame = frame;
        if (frame >= player.num_frame) {
            frame = 0;
        }
        playerRect.x = CENTER_X + static_cast<int>((last_x + move_frame * (x - last_x)/(NUM_FRAME_WALK - 1)) * TILE_SIZE);
        playerRect.y = CENTER_Y + static_cast<int>((last_y + move_frame * (y - last_y)/(NUM_FRAME_WALK - 1)) * TILE_SIZE);
        if (abs(playerRect.x - CENTER_X - last_x * TILE_SIZE) > TILE_SIZE) playerRect.x = CENTER_X + x * TILE_SIZE - 16;
        else playerRect.x -= 16;;
        if (abs(playerRect.y - CENTER_Y - last_y * TILE_SIZE) > TILE_SIZE) playerRect.y = CENTER_Y + y * TILE_SIZE - 16;
        else playerRect.y -= 16;
        if (timer >= player.speed_frame) {
            timer = 0;
            walk = false;
        }
    }
    player_frame  = { SIZE_TEXTURE_PLAYER * player.num_texture + player.size_gap * SIZE_TEXTURE_PLAYER * frame,  SIZE_TEXTURE_PLAYER * direct, PLAYER_SIZE , PLAYER_SIZE };
    if (walk) SDL_Log("%d %d %d", player_frame.x,player_frame.y, frame);
    SDL_RenderCopy(renderer, player.IMG, &player_frame, &playerRect);
};

int Player::getBombLimit() const {
    return bombLimit;
}

float Player::getX() const {
    return x;
}

float Player::getY() const{
    return y;
}

float Player::get_last_x() const {
    return last_x;
}

float Player::get_last_y() const {
    return last_y;
}

