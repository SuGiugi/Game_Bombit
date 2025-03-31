#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

class Player {
public:
    Player(float startX, float startY);
    void move(float dx, float dy);
    int getBombLimit() const; //Added method to retrieve bomb limit.
    float getX() const;
    float getY() const;
    float get_last_x() const;
    float get_last_y() const;
    float getSpeed() const { return speed; }
    void setXY(const float& newX,const float& newY) { x = newX;  y = newY; }
    void set_last_xy(const float& newX, const float& newY) { last_x = newX;  last_y = newY; }
    void set_direct(const int& a) {direct = a;};
    int getDirect() const { return direct; }
    void render_player(SDL_Renderer* renderer, SDL_Texture* player_texture, SDL_Texture* player_walk_texture,SDL_Texture* heart, bool &walk);
    void set_time() {timer = 0;};
    void heal() {
        if (health < 2) health++;
    };
    void hurt() {
        if (health > 0) health--;
    }

private:
    struct STATUS {
        SDL_Texture* IMG;
        int speed_frame;
        int num_frame;
        int num_texture;
        int size_gap;
        STATUS(SDL_Texture* IMG,const int speed_frame,const int num_frame,const int num_texture,const int size_gap) : IMG(IMG), speed_frame(speed_frame),num_frame(num_frame),num_texture(num_texture),size_gap(size_gap) {};
    };
    int health = 2;
    int timer;
    float last_x;
    float last_y;
    int direct;
    float x;
    float y;
    float speed;
    int bombLimit; //Maximum bombs player can place
};

#endif