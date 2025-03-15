#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
    Player(int startX, int startY);
    void move(int dx, int dy);
    int getBombLimit() const; //Added method to retrieve bomb limit.
    int getX() const;
    int getY() const;

private:
    int x;
    int y;
    int speed;
    int bombLimit; //Maximum bombs player can place
};

#endif