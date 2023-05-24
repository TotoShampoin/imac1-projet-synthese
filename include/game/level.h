#pragma once
#include <vector>

#include "logic/physics.h"
#include "IHM/objects.h"
#include "logic/vec3.h"
#include "game/bonus.h"

#define LEVEL_WIDTH 1.4
#define LEVEL_HEIGHT 1

typedef PhysicsAABB Obstacle;

struct Level {
    float length;
    std::vector<Obstacle> walls;
    std::vector<Obstacle> obstacles;
    std::vector<Bonus> bonus;

    Level(float length);
    Level(const char* file);
};

Mesh makeWallMesh();
