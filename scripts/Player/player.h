#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

#include "../Info/constant.h"
#include <bits/stdc++.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
using namespace std;

class Map;

class Player {
public:
    Player(int startX, int startY);
    void update(const int &direction, const int &next_x, const int &next_y, Map& map);
    void Death(const int &a){death = a;};
    void setXY(const int& newX,const int& newY) { x = newX;  y = newY; }
    void set_last_xy(const int& newX, const int& newY) { last_x = newX;  last_y = newY; }
    void set_direct(const int& a) {direct = a;}
    void render_player(SDL_Renderer* renderer);
    void set_time() {timer = 0;}
    void set_time_imortal()  {time_immortal = 30;} // cai thoi gian bat tu

    int getBombLimit() const; //Added method to retrieve bomb limit.
    int getX() const;
    int getY() const;
    int get_last_x() const;
    int get_last_y() const;
    int isDeath()const {return death;}
    int get_health() const {return health;}

    // cho thong so khien, kich thuoc no, so bom
    void get_status(int &a, int &b, int &c) const {

        a = shield;
        b = size_explode;
        c = bombLimit;
    }

    // hoi mau
    void heal(const string &c) {
        if (c == "full") {
            health = 2;
            bombLimit = reset[0];
            shield = reset[1];
            size_explode = reset[2];
        }
        if (health < 2) health++;
    };

    // gay sat thuong cho nhan vat
    void hurt(const string &c) {
        if (time_immortal < 0) {
            if (c == "piercing") health--;
            else if (shield > 0) shield --;
            else if (health > 0) {
                Mix_PlayChannel(-1, gOuch, 0);
                health--;
                time_immortal = 30;
            }
        }

    };
    //dieu chinh thong so cua nguoi choi ben ngoai MainMenu
    void setup(const int &x, const int &y) {
        switch (y) {
            case 0: if (x == 0 && bombLimit > 1) bombLimit--;
                    else if (x == 1 && bombLimit < 5) bombLimit++;
            break;
            case 1: if (x == 0 && shield > 0) shield--;
                    else if (x == 1 && shield < 5) shield++;
            break;
            case 2: if (x == 0 && size_explode > 2) size_explode--;
                    else if (x == 1 && size_explode <= 5) size_explode++;
            break;
            default: break;
        }
    }

    // xoa am thanh
    void close () {
        Mix_FreeChunk(gOuch);
        Mix_FreeChunk(gCollect);
        gOuch = NULL;
        gCollect = NULL;
    }

    int size() const {return size_explode;};

    // luu thong so sau khi chinh sua
    void Start_game(){
        reset[0] = bombLimit;
        reset[1] = shield;
        reset[2] = size_explode;
    }

    bool loadTexture(const string &filePath, const string &id, SDL_Renderer* renderer);

private:
    void write_status(SDL_Renderer* renderer,const int &number,const int &size);
    struct STATUS {
        string IMG;
        int speed_frame;
        int num_frame;
        STATUS(string IMG,const int speed_frame,const int num_frame) : IMG(IMG), speed_frame(speed_frame),num_frame(num_frame) {};
    };
    int reset[3] = {2, 0, 2};
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
    Mix_Chunk* gCollect = nullptr;
    Mix_Chunk* gOuch = nullptr;
};

#endif