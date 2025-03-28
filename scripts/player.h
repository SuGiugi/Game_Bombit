#ifndef PLAYER_H
#define PLAYER_H

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

private:
    float last_x;
    float last_y;
    int direct;
    float x;
    float y;
    float speed;
    int bombLimit; //Maximum bombs player can place
};

#endif