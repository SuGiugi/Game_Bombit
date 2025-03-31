#include "enemy.h"
#include "../map.h"
#include "../game.h"
#include "../constant.h"
#include "../render/resources.h"
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <cmath>
#include <queue>

Enemy::Enemy(float X, float Y, SDL_Renderer* renderer) :
    x0(X), y0(Y), speed(SPEED_PLAYER), texture(nullptr), timer(60), find(1), save(0), time_skill(0), use(false), time_frame(0), walk(false), cast(false), death(0) {
    texture = loadTexture("assets/images/enemies/Slime3_Idle.png", renderer);
    walk_texture = loadTexture("assets/images/enemies/Slime3_Walk.png", renderer);
    attack_texture = loadTexture("assets/images/enemies/Slime3_Attack.png", renderer);
    dead_texture = loadTexture("assets/images/enemies/Slime3_Death.png", renderer);
    ground_texture = loadTexture("assets/images/enemies/Slime3_Effect.png", renderer);
    if (texture == nullptr) {
        SDL_Log("Failed to load enemy texture!");
         //Don't try to render if we don't have a texture.
        return;
    }
    //Seed the random number generator (only once)
    static bool seeded = false;
    if(!seeded)
    {
        srand(time(NULL));
        seeded = true;
    }
}

Enemy::~Enemy() {
    //SDL_DestroyTexture(texture);
}

bool Enemy::is_valid(const Map& map,const int& x,const int& y) const {
    return (x >= 0) && (x < map.getWidth()) && (y >= 0) && (y < map.getHeight()) && (!(map.limit(x, y) > '2'));
}


void Enemy::update(int target_x, int target_y, const Map& map) {
    timer--;
    time_skill--;
    if (timer <= 0 && death == 0) {
        timer = ENEMIES_DELAY;
        int dx[] = {-1, 0, 1, 0};
        int dy[] = {0, -1, 0, 1};
        double vx = target_x - x0;
        double vy = target_y - y0;
        double distance = hypot(vx, vy);
        //SDL_Log("%d %d %d %d %d %d %d %d",x0, y0, save, last_x, last_y, find, next_dx, next_dy);
        if (vx < 0 && vy <= 0 && save != 0) {
            if (save == 1) find = 1;
            else find = 0;
            save = 0;
        } else if (vx >= 0 && vy < 0 && save != 1) {
            if (save == 2) find = 2;
            else find = 1;
            save = 1;
        } else if (vx > 0 && vy >= 0 && save != 2) {
            if (save == 3) find = 3;
            else find = 2;
            save = 2;
        } else if (vx <= 0 && vy > 0 && save != 3) {
            if (save == 0) find = 0;
            else find = 3;
            save = 3;
        }
        int cnt = 0;
        while (!is_valid(map,x0 + dx[find],y0 + dy[find]) && cnt <= 5) {
            find = (find + 1)%4;
            cnt++;
        }

        next_dx = dx[find];
        next_dy = dy[find];
        if (cnt >= 5) {
            next_dx = 0;
            next_dy = 0;
        };
        switch (find) {
            case 0: direct = 2;
            break;
            case 1: direct = 1;
            break;
            case 2: direct = 3;
            break;
            case 3: direct = 0;
            break;
        }

        if (find != save) find = (find - 1)%4;
        if (find < 0) find = 4 + find;

        if (map.limit(x0 + next_dx, y0 + next_dy) == '2' || distance <= 1) {
            if (time_skill < 0) {
                use = true;
                time_skill = 30;
                time_frame = 0;
            }
            last_x = 0;
            last_y = 0;
        } else {
            last_x = x0;
            last_y = y0;
            x0 += next_dx;
            y0 += next_dy;
            walk = true;
            time_frame = 0;
        }
    }



}


void Enemy::render(SDL_Renderer* renderer) {
    time_frame++;
    SDL_Rect enemy_frame;
    SDL_Rect enemyRect= {x0 * TILE_SIZE + CENTER_X - 16,y0 * TILE_SIZE + CENTER_Y - 16, SIZE_TEXTURE_PLAYER, SIZE_TEXTURE_PLAYER};
    int frame;
    STATUS enemy = {texture, ENEMIES_DELAY, NUM_FRAME_IDLE, 0, 1};
    if (!walk || death > 0) {
        if (death == 2) enemy = {dead_texture, ENEMIES_DEATH_SPEED, NUM_FRAME_DEATH, 0, 1};
        else if (use) enemy = {attack_texture, ENEMIES_ATTACK_SPEED, NUM_FRAME_ATTACK, 0, 1};
        else enemy = {texture, ENEMIES_DELAY, NUM_FRAME_IDLE, 0, 1};
        frame = time_frame/(enemy.speed_frame/enemy.num_frame);
        if (use && frame > NUM_FRAME_ATTACK - 2) cast = true;
        if (frame >= enemy.num_frame) {
            use = false;
            cast = false;
            if (death == 2) {
                death = 3;
                frame = 9;
            }
            else {
                frame = 0;
                time_frame = 0;
            }
        }
    } else {
        enemy = {walk_texture, ENEMIES_SPEED_FRAME, NUM_FRAME_WALK, 0, 1};
        frame = time_frame/(enemy.speed_frame/enemy.num_frame);
        float move_frame = frame;
        if (frame >= enemy.num_frame) {
            frame = 0;
        }
        enemyRect.x = CENTER_X + static_cast<int>((last_x + move_frame * (x0 - last_x)/(enemy.num_frame - 1)) * TILE_SIZE);
        enemyRect.y = CENTER_Y + static_cast<int>((last_y + move_frame * (y0 - last_y)/(enemy.num_frame - 1)) * TILE_SIZE);
        if (abs(enemyRect.x - CENTER_X - last_x * TILE_SIZE) > TILE_SIZE) enemyRect.x = CENTER_X + x0 * TILE_SIZE - 16;
        else enemyRect.x -= 16;
        if (abs(enemyRect.y - CENTER_Y - last_y * TILE_SIZE) > TILE_SIZE) enemyRect.y = CENTER_Y + y0 * TILE_SIZE - 16;
        else enemyRect.y -= 16;
        if (time_frame >= enemy.speed_frame) {
            time_frame = 0;
            walk =  false;
        }
    }
    if (use) {
        int dx[] = {-1, 0, 1, 0};
        int dy[] = {0, -1, 0, 1};
        for (int i = 0; i < 4; i++) {
            SDL_Rect effect = { CENTER_X + x0 * TILE_SIZE + dx[i] * TILE_SIZE, CENTER_Y + y0 * TILE_SIZE + dy[i] * TILE_SIZE, PLAYER_SIZE/2, PLAYER_SIZE/2 };
            //SDL_SetRenderDrawColor(renderer, 255, 152, 23, 255);
            //SDL_RenderFillRect(renderer, &effect);
            SDL_RenderCopy(renderer, ground_texture, NULL, &effect);
        }
    }
    enemy_frame  = { SIZE_TEXTURE_PLAYER * enemy.num_texture + enemy.size_gap * SIZE_TEXTURE_PLAYER * frame,  SIZE_TEXTURE_PLAYER * direct, PLAYER_SIZE , PLAYER_SIZE };
    if (enemy.IMG) {
        SDL_RenderCopy(renderer, enemy.IMG, &enemy_frame, &enemyRect);
    }

}

double Enemy::getX() const {
    return x0;
}

double Enemy::getY() const {
    return y0;
}

bool Enemy::get_skill() const {
    return cast;
}


