#pragma once
#include <vector>

#include "IHM/objects.h"
#include "logic/vec3.h"
// #include "game/bonus.h"

#define LEVEL_WIDTH 1.4
#define LEVEL_HEIGHT 1

struct Obstacle {
    Vec3f position;
    Vec3f size;
};

struct Level {
    float length;
    std::vector<Obstacle> walls;
    std::vector<Obstacle> obstacles;
    // std::vector<Bonus> bonus;

    Level(float length);
};

Mesh makeWallMesh();
