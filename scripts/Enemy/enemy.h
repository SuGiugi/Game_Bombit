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
    double get_dx() const;
    double get_dy() const;
    void changeX(double a) {x0 = a;};
    void changeY(double a) {y0 = a;}
    bool is_valid(const Map& map,const int& x,const int& y) const;

private:
    struct Node {
        int x, y;
        int dist;
        int step;
        Node(int x, int y, int dist, int step): x(x), y(y), dist(dist), step(step) {};
    };

    Logic logic;
    int cnt;
    int step;
    int save;
    int find;
    int next_dx;
    int next_dy;
    double x0;          // Enemy's x position (pixel coordinates)
    double y0;          // Enemy's y position (pixel coordinates)
    double speed;          // Enemy's movement speed
    SDL_Texture* texture;   // Enemy's texture
    int timer;
};

#endif