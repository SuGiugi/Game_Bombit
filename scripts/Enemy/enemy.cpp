#include "enemy.h"
#include "../Map/map.h"
#include "../game.h"
#include "../Info/constant.h"
#include "../Render/resources.h"
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <cmath>

// Khoi tao dich
Enemy::Enemy(int id, int X, int Y, SDL_Renderer* renderer) :
    x0(X), y0(Y), speed(SPEED_PLAYER), texture(nullptr), timer(60), find(1), save(0), time_skill(0), use(false),
    time_frame(0), walk(false), cast(false), death(0), flip(SDL_FLIP_NONE), enemyID(id) {
    //Seed the random number generator (only once)
    static bool seeded = false;
}

// kiem tra dich co di vao duoc hay khong
bool Enemy::is_valid(const Map& map,const int& x,const int& y) const {
    return (x >= 0) && (x < map.getWidth()) && (y >= 0) && (y < map.getHeight()) &&  (!(map.limit(x, y) == '3'));
}


void Enemy::update(int target_x, int target_y, Map& map) {
    timer--;
    time_skill--;
    // cai 4 huong
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, -1, 0, 1};
    if (timer <= 0 && death == 0 && use == false) {
        hurt = false;
        cast = false;
        timer = ENEMIES_DELAY[enemyID];
        // xac dinh goc 1/4 cua nguoi choi theo truc oxy de di chuyen
        double vx = target_x - x0;
        double vy = target_y - y0;
        double distance = hypot(vx, vy);
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
        // kiem tra huong co the di chuyen
        while (!is_valid(map,x0 + dx[find],y0 + dy[find]) && cnt <= 5) {
            find = (find + 1)%4;
            cnt++;
        }

        // xac dinh huong di chuyen de tao texture
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
            default: direct = 0;
            break;
        }

        if (find != save) find = (find - 1)%4;
        if (find < 0) find = 4 + find;

        // kiem tra vat can va khoang cach de tung chieu
        if (map.limit(x0 + next_dx, y0 + next_dy) == '2' || distance <= DANGER[enemyID]) {
            if (time_skill < 0) {
                use = true;
                time_skill = TIME_SKILL[enemyID];
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

    // cast chieu tuy theo loai quai vat
    if (cast) {
        if (enemyID == 1) { //Type 1
            for (int i = 0; i < 4; i++) {
                int p_x = x0 + dx[i];
                int p_y = y0 + dy[i];
                if (map.limit(p_x, p_y) == '2') map.Create_map('0', p_x, p_y);
            }
        } else if (enemyID == 2) { // Type 2
            for (int i = 0; i <= 2; i++) {
                int p_x = x0 + next_dx * i;
                int p_y = y0 + next_dy * i;
                srand(time(NULL) + i);
                int random = rand()%8;
                if (map.limit(p_x, p_y) == '2') map.Create_map('0', p_x, p_y);
                if (random == 1 && map.limit(p_x, p_y) == '0') map.Create_map('8', p_x, p_y);
            }
        }  else { //Type 3
            int p_x = x0 + next_dx;
            int p_y = y0 + next_dy;
            if (map.limit(p_x, p_y) == '2') map.Create_map('0', p_x, p_y);
        }

    }


}

// render hinh anh
void Enemy::render(SDL_Renderer* renderer, int target_x, int target_y) {
    time_frame++;
    SDL_Rect enemyRect= {x0 * TILE_SIZE + CENTER_X - 16,y0 * TILE_SIZE + CENTER_Y - 16, SIZE_TEXTURE_PLAYER, SIZE_TEXTURE_PLAYER};
    int frame;
    string c = "enemy" + to_string(enemyID);
    STATUS enemy = {c, ENEMIES_DELAY[enemyID], NUM_FRAME_IDLE[enemyID]};
    // xac dinh loai quai vat, dang di hay dung, dang dung chieu hay chet
    if (!walk || death > 0) {
        if (death == 2) enemy = {c + "_death", ENEMIES_DEATH_SPEED, NUM_FRAME_DEATH};
        else if (use) enemy = {c + "_attack", ENEMIES_ATTACK_SPEED[enemyID], NUM_FRAME_ATTACK[enemyID]};
        frame = time_frame/(enemy.speed_frame/enemy.num_frame);
        if (use && frame > NUM_FRAME_ATTACK[enemyID] - 4) cast = true;
        if (frame >= enemy.num_frame) {
            use = false;
            kill = false;
            if (death == 2) {
                death = 3;
                frame = NUM_FRAME_ATTACK[enemyID];
            }
            else {
                frame = 0;
                time_frame = 0;
            }
        }
    } else {
        enemy = {c + "_walk", ENEMIES_SPEED_FRAME[enemyID], NUM_FRAME_WALK[enemyID]};
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
    //use skill
    if (cast) {
        if (target_x == x0 && target_y == y0) {
            kill = true;
        }
        if (enemyID == 1) {
            int dx[] = {-1, 0, 1, 0};
            int dy[] = {0, -1, 0, 1};
            for (int i = 0; i < 4; i++) {
                if (target_x == x0 + dx[i] && target_y == y0 + dy[i]) {
                    kill = true;
                }
                SDL_Rect effect = { CENTER_X + x0 * TILE_SIZE + dx[i] * TILE_SIZE, CENTER_Y + y0 * TILE_SIZE + dy[i] * TILE_SIZE, PLAYER_SIZE/2, PLAYER_SIZE/2 };

                Resources::Instance()->render(c + "_effect",
                effect.x, effect.y,
                effect.w, effect.h,
                48,48,
                renderer, flip);
            }
        } else if (enemyID == 2){
            int dx[] = {0, 0, -1, 1};
            int dy[] = {1, -1, 0, 0};
            for (int i = 1; i <= 2; i++) {
                if (target_x == x0 + dx[direct] * i && target_y == y0 + dy[direct] * i) {
                    kill = true;
                }
                SDL_Rect effect = { CENTER_X + x0 * TILE_SIZE + i * dx[direct] * TILE_SIZE, CENTER_Y + y0 * TILE_SIZE + i * dy[direct] * TILE_SIZE, PLAYER_SIZE/2, PLAYER_SIZE/2 };

                Resources::Instance()->render(c + "_effect",
                effect.x, effect.y,
                effect.w, effect.h,
                360,360,
                renderer, flip);
            }
        } else {
            int dx[] = {0, 0, -1, 1};
            int dy[] = {1, -1, 0, 0};
                if (target_x == x0 + dx[direct] && target_y == y0 + dy[direct]) {
                    kill = true;
                }
                SDL_Rect effect = { CENTER_X + x0 * TILE_SIZE +  dx[direct] * TILE_SIZE,
                    CENTER_Y + y0 * TILE_SIZE + dy[direct] * TILE_SIZE, PLAYER_SIZE/2, PLAYER_SIZE/2 };

                Resources::Instance()->render(c + "_effect",
                effect.x, effect.y,
                effect.w, effect.h,
                360,360,
                renderer, flip);

        }
    }
    // renderEnemy
    textureID = enemy.IMG;
    Resources::Instance()->renderFrame(
        textureID,
        enemyRect.x, enemyRect.y,
        enemyRect.w, enemyRect.h,
        direct, frame,
        64, 64,
        renderer,
        flip
        );
}

int Enemy::getX() const {
    return x0;
}

int Enemy::getY() const {
    return y0;
}




