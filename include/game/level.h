#pragma once
#include <vector>

#include "vec3.h"
#include "bonus.h"

struct Obstacle {
    Vec3f position;
    Vec3f size;
};

struct Level {
    float length;
    std::vector<Obstacle> obstacles;
    std::vector<Bonus> bonus;
};

