#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>

class Enemy {
public:
    Enemy(float startX, float startY, SDL_Renderer* renderer);
    ~Enemy();

    void update();
    void render(SDL_Renderer* renderer);

    double getX() const;
    double getY() const;
    double get_dx() const;
    double get_dy() const;
    void changeX(double a) {x = a;};
    void changeY(double a) {y = a;}
    bool isDeath() const {return Death;};
    void Delete() {Death = true;};

private:
    bool Death;
    double dx;
    double dy;
    double x;          // Enemy's x position (pixel coordinates)
    double y;          // Enemy's y position (pixel coordinates)
    double speed;          // Enemy's movement speed
    SDL_Texture* texture;   // Enemy's texture
    int timer;
};

#endif