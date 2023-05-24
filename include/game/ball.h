#pragma once
#include "logic/vec3.h"
#include "IHM/objects.h"
#include "logic/physics.h"

#include <vector>

struct Ball {
    Vec3f position = Vec3f(0,0,0);
    Vec3f speed_dir = Vec3f(0,0,0);
    float speed = 2;
    float radius = 0.25;

    void move(float deltaTime);
    bool collide(std::vector<Obstacle>&);
};

Mesh makeBallMesh();

