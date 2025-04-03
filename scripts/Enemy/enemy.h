#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include "../Info/logic.h"
#include <vector>
#include <string>
using namespace std;

class Map;

class Enemy {
public:
    Enemy(int id, int X, int Y, SDL_Renderer* renderer);

    void update(int target_x, int target_y, Map& map);
    void render(SDL_Renderer* renderer, int target_x,int target_y);
    void changeX(double a) {x0 = a;};
    void changeY(double a) {y0 = a;}
    void Death() {death = 2;};
    void hurt_player() {hurt = true;}

    int getX() const;
    int getY() const;
    int isDeath() const {return death;}
    int ID() const {return enemyID;}

    bool is_valid(const Map& map,const int& x,const int& y) const;
    bool is_kill() const {return kill;}
    bool is_hurt() const {return hurt;}



private:
    struct STATUS {
        string IMG;
        int speed_frame;
        int num_frame;
        STATUS(string IMG,const int speed_frame,const int num_frame) : IMG(IMG), speed_frame(speed_frame),num_frame(num_frame) {};
    };

    string textureID;
    SDL_RendererFlip flip;
    bool hurt = false;
    bool kill = false;
    bool walk;
    bool use;
    bool cast;

    int enemyID;
    int death;
    int direct;
    int save;
    int find;
    int next_dx;
    int next_dy;
    int last_x;
    int last_y;
    int timer;
    int time_frame;
    int time_skill;
    int x0;          // Enemy's x position (pixel coordinates)
    int y0;          // Enemy's y position (pixel coordinates)

    double speed;          // Enemy's movement speed

    SDL_Texture* texture;   // Enemy's texture
    SDL_Texture* walk_texture;
    SDL_Texture* ground_texture;
    SDL_Texture* run_texture;
    SDL_Texture* attack_texture;
    SDL_Texture* dead_texture;
};

#endif