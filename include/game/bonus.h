#pragma once
#include <map>
#include <functional>

#include "logic/vec3.h"
#include "IHM/objects.h"

#include "game/player.h"

struct Type {
    std::function<void (Player&)> start;
    std::function<void (Player&)> end;
    float time_in_second = 10.;
    Vec3f size {.25, .25, .25};
};

enum TypeID {
    VICTORY,
    SLOW_BALL,
    LARGE_RACKET,
};
static std::vector<Type> type_table;

struct Bonus {
    Type* specs = &type_table[VICTORY];
    Vec3f position;
    float time_remaining = 0;

    bool is_active = false;
    bool is_picked = false;
    Player* player = nullptr;

    Bonus(Type& specs, Vec3f position);
    Bonus(TypeID type_id, Vec3f position);

    void update(float delta_time);

    void start();
    void end();
    void bePicked(Player&);
};

void initTypeTable();

