#ifndef BOMB_H
#define BOMB_H

class Bomb {
public:
    Bomb(int x, int y);
    void update();
    bool isExploded() const;
    int getX() const;
    int getY() const;
    int gettime() const {
        return timer;
    }

private:
    int x;
    int y;
    int timer;
    bool exploded;
};

#endif