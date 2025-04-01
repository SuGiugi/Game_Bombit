#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

#include "constant.h"
#include <bits/stdc++.h>
using namespace std;

class Map;

class Player {
public:
    Player(int startX, int startY);
    void update(const int &direction, const int &next_x, const int &next_y, Map& map);
    int getBombLimit() const; //Added method to retrieve bomb limit.
    int getX() const;
    int getY() const;
    int get_last_x() const;
    int get_last_y() const;
    float getSpeed() const { return speed; }
    void setXY(const float& newX,const float& newY) { x = newX;  y = newY; }
    void set_last_xy(const float& newX, const float& newY) { last_x = newX;  last_y = newY; }
    void set_direct(const int& a) {direct = a;};
    int getDirect() const { return direct; }
    void render_player(SDL_Renderer* renderer);
    void set_time() {timer = 0;};
    void heal() {
        if (health < 2) health++;
    };
    void hurt() {
        if (health > 0) health--;
    }
    void increase_explode() {
        if (size_explode <= 5) size_explode++;
    }
    int size() {return size_explode;};

    bool loadTexture(const string &filePath, const string &id, SDL_Renderer* renderer);

private:
    struct STATUS {
        string IMG;
        int speed_frame;
        int num_frame;
        int num_texture;
        int size_gap;
        STATUS(string IMG,const int speed_frame,const int num_frame,const int num_texture,const int size_gap) : IMG(IMG), speed_frame(speed_frame),num_frame(num_frame),num_texture(num_texture),size_gap(size_gap) {};
    };
    int bombLimit; //Maximum bombs player can place
    int size_explode = SIZE_EXPLODE;
    int direct;
    int health = 2;
    int timer;
    int last_x;
    int last_y;
    int x;
    int y;
    int dx, dy;
    int speed;
    bool walk = false;
    string textureID;
    SDL_RendererFlip flip;
};

#endif