//
// Created by Admin on 3/14/2025.
//
#include "map.h"

#ifndef CONSTANT_H
#define CONSTANT_H

//texture
const int SCREEN_WIDTH  = 900;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE     = 32;
const int SIZE_TEXTURE_PLAYER = 64;
const int NUM_BACKGROUND  = 4;

//frame
const int NUM_FRAME_IDLE = 6;
const int NUM_FRAME_WALK = 7;
const int NUM_FRAME_ATTACK[] = {0, 9, 11};
const int NUM_FRAMES = 5;
const int NUM_FRAME_DEATH = 10;

//delay - time
const int TIME_EXPLODE = 150;
const int PRESS_DELAY = 15;
const int ENEMIES_DELAY[] = {0, 25, 35};
const int ENEMIES_SPEED_FRAME = 20;
const int ENEMIES_ATTACK_SPEED[] = {0, 25, 30};
const int ENEMIES_DEATH_SPEED = 50;
const int PLAYER_WALK_FRAME = 14;
const int TIME_1SECOND = 1000;
const int TIME_SKILL[] = {0, 105, 120};
const int TIME_POPUP = 60;


//size
const int SIZE_EXPLODE = 2;
const int PLAYER_SIZE = 64;
const int SPEED_PLAYER = 1; // 0.2 or 0.5
const float FIXED = 0.5;
const int CENTER_X = (SCREEN_WIDTH - 17 * TILE_SIZE) / 2;
const int CENTER_Y = (SCREEN_HEIGHT - 17 * TILE_SIZE) / 2;



#endif //CONSTANT_H
