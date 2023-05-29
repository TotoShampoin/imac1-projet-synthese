#pragma once
#include <vector>

#include "logic/physics.h"
#include "IHM/objects.h"
#include "logic/vec3.h"
#include "game/bonus.h"

#define DEFAULT_LEVEL_WIDTH 1.4
#define DEFAULT_LEVEL_HEIGHT 1

struct Level {
    float length;
    float width = DEFAULT_LEVEL_WIDTH, height = DEFAULT_LEVEL_HEIGHT;
    std::vector<Obstacle> walls;
    std::vector<Obstacle> obstacles;
    std::vector<Bonus> bonus;

    Level(float length);
    Level(const char* file);

    void addDefaultElements();

    std::vector<Obstacle> getAllObstacles();
};

Mesh makeWallMesh();
