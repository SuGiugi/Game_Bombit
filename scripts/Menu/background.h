//
// Created by Admin on 3/30/2025.
//

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL.h>
#include "../constant.h"
class Background {
    public:
    void load(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);

    private:
     SDL_Texture* background;
};

#endif //BACKGROUND_H
