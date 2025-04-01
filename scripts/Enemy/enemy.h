#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include "../logic.h"
#include <vector>
#include <string>
using namespace std;

class Map;

class Enemy {
public:
    Enemy(int X, int Y, SDL_Renderer* renderer);
    ~Enemy();

    void update(int target_x, int target_y, const Map& map);
    void render(SDL_Renderer* renderer, int target_x,int target_y);

    double getX() const;
    double getY() const;
    bool get_skill()const;
    void changeX(double a) {x0 = a;};
    void changeY(double a) {y0 = a;}
    bool is_valid(const Map& map,const int& x,const int& y) const;
    void Death() {death = 2;};
    int isDeath() const {return death;};
    bool is_kill() const {return kill;};
    bool is_hurt() const {return hurt;};
    void hurt_player() {hurt = true;};

private:
    struct STATUS {
        string IMG;
        int speed_frame;
        int num_frame;
        int num_texture;
        int size_gap;
        STATUS(string IMG,const int speed_frame,const int num_frame,const int num_texture,const int size_gap) : IMG(IMG), speed_frame(speed_frame),num_frame(num_frame),num_texture(num_texture),size_gap(size_gap) {};
    };

    string textureID;
    SDL_RendererFlip flip;
    bool hurt = false;
    bool kill = false;
    bool walk;
    int death;
    int direct;
    bool use;
    bool cast;
    int save;
    int find;
    int next_dx;
    int last_x;
    int last_y;
    int next_dy;
    int x0;          // Enemy's x position (pixel coordinates)
    int y0;          // Enemy's y position (pixel coordinates)
    double speed;          // Enemy's movement speed
    SDL_Texture* texture;   // Enemy's texture
    SDL_Texture* walk_texture;
    SDL_Texture* ground_texture;
    SDL_Texture* run_texture;
    SDL_Texture* attack_texture;
    SDL_Texture* dead_texture;
    int timer;
    int time_frame;
    int time_skill;
};

#endif