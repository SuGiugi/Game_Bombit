#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

#include "constant.h"
#include <bits/stdc++.h>
#include <SDL_ttf.h>
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
    int isDeath()const {return death;};
    void Death(const int &a){death = a;};
    void setXY(const int& newX,const int& newY) { x = newX;  y = newY; }
    void set_last_xy(const int& newX, const int& newY) { last_x = newX;  last_y = newY; }
    void set_direct(const int& a) {direct = a;};
    int getDirect() const { return direct; }
    void render_player(SDL_Renderer* renderer);
    void set_time() {timer = 0;};
    int get_health() const {return health;};
    void heal(const string &c) {
        if (c == "full") {
            health = 2;
        }
        if (health < 2) health++;
    };
    void hurt() {
        if (time_immortal < 0) {
            if (shield > 0) shield --;
            else if (health > 0) {
                health--;
                time_immortal = 30;
            }
        }

    }
    void increase_explode() {
        if (size_explode <= 5) size_explode++;
    }
    int size() {return size_explode;};

    bool loadTexture(const string &filePath, const string &id, SDL_Renderer* renderer);

private:
    void write_status(SDL_Renderer* renderer,const int number,const int &size);
    struct STATUS {
        string IMG;
        int speed_frame;
        int num_frame;
        STATUS(string IMG,const int speed_frame,const int num_frame) : IMG(IMG), speed_frame(speed_frame),num_frame(num_frame) {};
    };
    int death = 0;
    int bombLimit; //Maximum bombs player can place
    int shield = 0;
    int size_explode = SIZE_EXPLODE;
    int direct;
    int health = 2;
    int timer;
    int time_immortal = 30;
    int last_x;
    int last_y;
    int x;
    int y;
    int dx, dy;
    int speed;
    bool walk = false;
    string textureID;
    SDL_RendererFlip flip;
    SDL_Color Color = {3, 3, 3, 255};
    TTF_Font* Font;
};

#endif