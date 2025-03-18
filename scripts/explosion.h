#ifndef EXPLOSION_H
#define EXPLOSION_H
#include <SDL.h>
#include <utility>
#include <vector>
#include "logic.h"

// Add your explosion class definition here
class Explosion {
public:
    std::pair<int, int> position[4] = {
        std::make_pair(0, 1),
        std::make_pair(1, 0),
        std::make_pair(0, -1),
        std::make_pair(-1, 0)
    };
    Explosion(int x, int y, SDL_Renderer* renderer);
    ~Explosion();

    void update();
    void render(SDL_Renderer* renderer, float px, float py);
    bool isFinished() const { return finished; }

private:
    int x;           // Explosion center x (tile coordinates)
    int y;           // Explosion center y (tile coordinates)
    int timer;       // Explosion timer (frames)
    bool finished;   // Flag to indicate if the explosion is finished
    SDL_Texture* texture; //The texture for each "frame" of the explosion.
    int currentFrame; //Which frame we are on.
    Logic logic;

    SDL_Rect explosionRects[5]; //Array for rectangles containing the texture frame.
};
#endif