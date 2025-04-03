//
// Created by Admin on 3/31/2025.
//

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include "../Player/player.h"
#include <vector>

enum class GAME_STATE;
class Game;
class Bomb;
class Map;
class InputHandler {
public:
    InputHandler();
    ~InputHandler() = default;

    void handleEvents(bool& running, Player& player, Game& game, GAME_STATE& currentState,Map& map, std::vector<Bomb> &bombs);

private:
    bool mouseButtonDown;
    bool UpKeyDown;
    bool DownKeyDown;
    bool LeftKeyDown;
    bool RightKeyDown;
    bool SpaceKeyDown;
};


#endif //INPUTHANDLER_H
