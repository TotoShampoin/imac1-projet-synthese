#pragma once
#include <vector>
#include <functional>

#include "logic/vec3.h"
#include "IHM/objects.h"

struct Type {
    Geometry& forme;
    std::function<void ()> start;
    std::function<void ()> end;
};

struct Bonus {
    Vec3f position;
    Type& specs;
};

struct TypeTable {
    std::vector<Type> types;
    Bonus createBonus(int type_id, Vec3f position);
};
