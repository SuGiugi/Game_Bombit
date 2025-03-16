#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
    Player(float startX, float startY);
    void move(float dx, float dy);
    int getBombLimit() const; //Added method to retrieve bomb limit.
    float getX() const;
    float getY() const;
    float getSpeed() const { return speed; }
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }

private:
    float x;
    float y;
    float speed;
    int bombLimit; //Maximum bombs player can place
};

#endif