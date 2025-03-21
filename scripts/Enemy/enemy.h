#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>

class Enemy {
public:
    Enemy(float startX, float startY, SDL_Renderer* renderer);
    ~Enemy();

    void update();
    void render(SDL_Renderer* renderer);

    float getX() const;
    float getY() const;

private:
    float x;          // Enemy's x position (pixel coordinates)
    float y;          // Enemy's y position (pixel coordinates)
    int speed;          // Enemy's movement speed
    SDL_Texture* texture;   // Enemy's texture
    int timer;
};

#endif