#ifndef EXPLOSION_H
#define EXPLOSION_H

// Add your explosion class definition here
class Explosion {
public:
    void update(int px, int py, int bx, int by, int size); //They die by bomb or not ?
    bool getExplode() const;
private:
    bool explode;
};
#endif