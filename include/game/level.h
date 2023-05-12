#pragma once
#include <vector>

#include "logic/vec3.h"
#include "bonus.h"

Mesh makeWallMesh();

struct Obstacle {
    Vec3f position;
    Vec3f size;
};

struct Level {
    float length = 20;
    std::vector<Obstacle> obstacles;
    std::vector<Bonus> bonus;

    Mesh wall_mesh = makeWallMesh();

    Level();
};

