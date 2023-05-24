#pragma once

#include "logic/vec3.h"
#include "IHM/objects.h"
#include "logic/physics.h"

#include <vector>

struct Racket {
    Vec3f position = Vec3f(0,0,0);
    Vec3f scale = Vec3f(0.3, 0.3, 0.3);
    bool hasBall = true;
    //Vec3f speed = Vec3f(0,0,0);
    //float radius = 0.25;

    //void move(float deltaTime);
    //bool collide(std::vector<Obstacle>&);
};

Geometry makeRacketMesh();

