#ifndef EXPLOSION_H
#define EXPLOSION_H
#include <SDL.h>
#include <utility>
#include <vector>
#include "../Info/logic.h"
#include "../Map/map.h"
#include <string>
using namespace std;

// Add your explosion class definition here
class Explosion {
public:
    Explosion(int x, int y, SDL_Renderer* renderer);

    void update();
    void render(SDL_Renderer* renderer,string id, int px, int py);
    bool isFinished() const { return finished; }
    int get_currentFrame() const {return currentFrame;};
    int get_X() const {return x;};
    int get_Y() const {return y;};
    bool get_hurt()const {return hurt;};
    void is_hurt() {hurt = true;};


private:
    int explosion_x;
    int explosion_y;
    int x;           // Explosion center x (tile coordinates)
    int y;           // Explosion center y (tile coordinates)
    int timer;       // Explosion timer (frames)
    bool finished;   // Flag to indicate if the explosion is finished
    bool hurt = false;
    int currentFrame; //Which frame we are on.

    Logic logic;

    SDL_Rect explosionRects[5]; //Array for rectangles containing the texture frame.
};
#endif