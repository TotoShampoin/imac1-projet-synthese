#include "game/bonus.h"
#include "IHM/objects.h"

void initTypeTable() {
    type_table[VICTORY] = Type {
        [](Player &P) {
            P.ball.speed /= 2;
        },
        [](Player &P) {
            P.ball.speed *= 2;
        },
        0, Vec3f(2, 2, .5)
    };
    type_table[SLOW_BALL] = Type {
        [](Player &P) {
            P.ball.speed /= 2;
        },
        [](Player &P) {
            P.ball.speed *= 2;
        }
    };
    type_table[LARGE_RACKET] = Type {
        [](Player &P) {
            P.racket.scale *= 1.5;
        },
        [](Player &P) {
            P.racket.scale /= 1.5;
        }
    };
}

Bonus::Bonus(TypeID type_id, Vec3f pos) {
    specs = type_table[type_id];
    position = pos;
    time_remaining = specs.time_in_second;
}

Bonus::Bonus(Type& sp, Vec3f pos) {
    specs = sp;
    position = pos;
    time_remaining = specs.time_in_second;
}

void Bonus::update(float delta_time) {
    if(is_active) {
        time_remaining -= delta_time;
        if(time_remaining <= 0) {
            end();
        }
    }
}

void Bonus::start() {
    specs.start(*player);
    is_active = true;
}
void Bonus::end() {
    specs.end(*player);
    player = nullptr;
    is_active = false;
}
void Bonus::bePicked(Player& P) {
    player = &P;
    is_picked = true;
    start();
}
