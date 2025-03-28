#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include "../logic.h"
#include <vector>
#include <string>

class Map;

class Enemy {
public:
    Enemy(float X, float Y, SDL_Renderer* renderer);
    ~Enemy();

    void update(int target_x, int target_y, const Map& map);
    void render(SDL_Renderer* renderer);

    double getX() const;
    double getY() const;
    bool get_skill()const;
    void changeX(double a) {x0 = a;};
    void changeY(double a) {y0 = a;}
    bool is_valid(const Map& map,const int& x,const int& y) const;

private:
    struct STATUS {
        SDL_Texture* IMG;
        int speed_frame;
        int num_frame;
        int num_texture;
        int size_gap;
        STATUS(SDL_Texture* IMG,const int speed_frame,const int num_frame,const int num_texture,const int size_gap) : IMG(IMG), speed_frame(speed_frame),num_frame(num_frame),num_texture(num_texture),size_gap(size_gap) {};
    };
    bool walk;
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
    SDL_Texture* run_texture;
    SDL_Texture* attack_texture;
    SDL_Texture* dead_texture;
    int timer;
    int time_frame;
    int time_skill;
};

#endif