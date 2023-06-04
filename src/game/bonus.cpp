#include "game/bonus.h"
#include "IHM/objects.h"

void initTypeTable() {
    static bool is_init = false;
    if(is_init) return;
    type_table.push_back(Type { // 0: VICTOIRE
        [](Player &P) {
            P.hasReachedEndLine = true;
        },
        [](Player &P) {},
        0, Vec3f(2, 2, .5),
        nullptr, true
    });
    type_table.push_back(Type { // 1: SLOW_BALL
        [](Player &P) {
            P.ball.speed /= 2;
        },
        [](Player &P) {
            P.ball.speed *= 2;
        },
        BONUS_DEFAULT_TIME, BONUS_DEFAULT_SIZE,
        new Geometry("assets/models/slow.mdl")
    });
    type_table.push_back(Type { // 2: LARGE_RACKET
        [](Player &P) {
            P.racket.scale *= 1.5;
        },
        [](Player &P) {
            P.racket.scale /= 1.5;
        },
        BONUS_DEFAULT_TIME, BONUS_DEFAULT_SIZE,
        new Geometry("assets/models/grow.mdl")
    });
    type_table.push_back(Type { // 3 : SNATCHER
        [](Player &P) {
            P.racket.canCatchBall = true;
        },
        [](Player &P) {
            P.racket.canCatchBall = false;
        },
        BONUS_DEFAULT_TIME, BONUS_DEFAULT_SIZE,
        new Geometry("assets/models/stick.mdl")
    });
    is_init = true;
}

Bonus::Bonus(TypeID type_id, Vec3f pos) {
    initTypeTable();
    specs = &type_table[type_id];
    position = pos;
    time_remaining = specs->time_in_second;
}

Bonus::Bonus(Type& sp, Vec3f pos) {
    specs = &sp;
    position = pos;
    time_remaining = specs->time_in_second;
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
    specs->start(*player);
    is_active = true;
}
void Bonus::end() {
    specs->end(*player);
    player = nullptr;
    is_active = false;
}
void Bonus::bePicked(Player& P) {
    player = &P;
    is_picked = true;
    start();
}

PhysicsAABB Bonus::getHitbox() {
    return PhysicsAABB (
        position - specs->size /2,
        position + specs->size /2
    );
}