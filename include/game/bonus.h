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
    Vec3f size {.35, .35, .35};
    bool is_victory = 0;
};

typedef unsigned int TypeID;
#define VICTORY         0
#define SLOW_BALL       1
#define LARGE_RACKET    2
#define SNATCHER        3

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
    PhysicsAABB getHitbox();
};

void initTypeTable();

