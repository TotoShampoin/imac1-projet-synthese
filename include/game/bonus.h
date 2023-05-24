#pragma once
#include <map>
#include <functional>

#include "logic/vec3.h"
#include "IHM/objects.h"

#include "game/ball.h"
#include "game/player.h"
#include "game/racket.h"

struct Type {
    std::function<void (Player&)> start;
    std::function<void (Player&)> end;
    float time_in_second = 10.;
    Vec3f size {.25, .25, .25};
};

struct Bonus {
    Type& specs;
    Vec3f position;
    float time_remaining;

    bool is_active = false;
    bool is_picked = false;
    Player* player = nullptr;

    void update(float delta_time);

    void start();
    void end();
    void bePicked(Player&);
};

enum TypeID {
    VICTORY,
    SLOW_BALL,
    LARGE_RACKET,
};
static std::vector<Type> type_table;

void initTypeTable();

Bonus createBonus(TypeID type_id, Vec3f position);
